#version 330 core

in vec2 UV;
in vec3 FragPos;
in vec3 Normal;

out vec4 fragColor;

uniform sampler2D texture_1;  // Diffuse
uniform sampler2D texture_2;  // Normal Map
uniform int use_normal_map;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

// --- Uniforms noi pentru ceata ---
uniform float max_distance; // d_max
uniform vec3 fog_color;     // C_fog (ex: vec3(0.5, 0.5, 0.5))


//--- Uniforms pentru licurici si spotlights ---
#define MAX_FIREFLIES 32

uniform int   uFireflyCount;
uniform vec3  uFireflyPos[MAX_FIREFLIES];
uniform vec3  uFireflyColor[MAX_FIREFLIES];
uniform float uFireflyIntensity[MAX_FIREFLIES];

uniform int uSpotCount;

uniform vec3  uSpotPos[2];
uniform vec3  uSpotDir[2];
uniform vec3  uSpotColor[2];
uniform float uSpotCutoff[2];
uniform float uSpotOuterCutoff[2];

uniform sampler2D cookieTexture;


vec3 PointLightPhong(vec3 lightPos, vec3 lightColor, float intensity, vec3 fragPos, vec3 N, vec3 V, vec3 albedo)
{
    vec3 L = normalize(lightPos - fragPos);

    float dist = 2 * length(lightPos - fragPos);
    float att = 1.0 / (1.0 + 0.35 * dist + 0.25 * dist * dist);

    float diff = max(dot(N, L), 0.0);

    // specular Phong (reflection)
    vec3 R = reflect(-L, N);
    float spec = pow(max(dot(V, R), 0.0), 12.0);

    vec3 diffuse  = diff * albedo * lightColor;
    vec3 specular = spec * lightColor * 0.08;

    return (diffuse + specular) * intensity * att;
}

vec2 ComputeCookieUV(vec3 lightPos, vec3 lightDir, vec3 fragPos, float alpha)
{
    // v = X - L
    vec3 v = fragPos - lightPos;

    // z^ = forward = lightDir normalizat
    vec3 zAxis = normalize(lightDir);

    // a = (0,1,0) daca nu e paralel cu z^, altfel (1,0,0)
    vec3 a = vec3(0, 1, 0);
    if (abs(dot(zAxis, a)) > 0.999)
        a = vec3(1, 0, 0);

    // x^ = normalize(a × z^)
    vec3 xAxis = normalize(cross(a, zAxis));

    // y^ = z^ × x^
    vec3 yAxis = cross(zAxis, xAxis);

    // proiectare in sistemul spotului:
    float x = dot(v, xAxis);
    float y = dot(v, yAxis);
    float z = dot(v, zAxis);

    // daca e in spatele spotului
    if (z <= 0.001)
        return vec2(-1.0);

    // p = (x/z, y/z)
    vec2 p = vec2(x / z, y / z);

    // R = tan(alpha)
    float R = tan(alpha);

    // n = p / R  => normalizare in [-1,1]
    vec2 n = p / R;

    // t = n/2 + 1/2  => mapare in [0,1]
    vec2 uv = n * 0.5 + 0.5;

    return uv;
}

vec3 SpotLightPhong(vec3 lightPos, vec3 lightDir, vec3 lightColor, float cutoff, float outerCutoff, vec3 fragPos, vec3 N, vec3 V, vec3 albedo)
{
    vec3 toLight = lightPos - fragPos;
    float dist = length(toLight);
    vec3 L = toLight / dist;

    // spotlight check
    float theta = dot(normalize(-L), normalize(lightDir)); // cat de aliniat e fragmentul cu directia spotului

    // smooth edge
    float epsilon = cutoff - outerCutoff;
    float spotIntensity = clamp((theta - outerCutoff) / epsilon, 0.0, 1.0);

    if (spotIntensity <= 0.0)
        return vec3(0);

    // attenuation
    float att = 1.0 / (1.0 + 0.09 * dist + 0.032 * dist * dist);

    // diffuse
    float diff = max(dot(N, L), 0.0);

    // specular (Blinn sau reflect)
    vec3 H = normalize(L + V);
    float spec = pow(max(dot(N, H), 0.0), 32.0);

    vec3 diffuse  = diff * albedo * lightColor;
    vec3 specular = spec * lightColor * 0.25;

    float alpha = acos(outerCutoff);
    vec2 cookieUV = ComputeCookieUV(lightPos, lightDir, fragPos, alpha);

    // daca uv invalid sau in afara
    if (cookieUV.x < 0.0 || cookieUV.x > 1.0 || cookieUV.y < 0.0 || cookieUV.y > 1.0)
        return vec3(0);

    // esantionare cookie
    vec3 cookie = texture(cookieTexture, cookieUV).rgb;

    // modulare contributie lumina
    vec3 base = (diffuse + specular) * att * spotIntensity;
    return base * cookie;
}

void main()
{
    vec3 albedo = texture(texture_1, UV).rgb;
    vec3 N = normalize(Normal);

    if (use_normal_map == 1)
    {
        vec3 nTex = texture(texture_2, UV).rgb;
        nTex = nTex * 2.0 - 1.0;

        vec3 T = vec3(1, 0, 0);
        vec3 B = vec3(0, 0, 1);
        vec3 NN = vec3(0, 1, 0);

        mat3 TBN = mat3(T, B, NN);
        N = normalize(TBN * nTex);
    }

    // Iluminare Blinn-Phong
    vec3 L = normalize(lightPos - FragPos);
    vec3 V = normalize(viewPos - FragPos);
    vec3 H = normalize(L + V);

    float diff = max(dot(N, L), 0.0);
    float spec = pow(max(dot(N, H), 0.0), 32.0);

    vec3 ambient  = 0.08 * albedo;
    vec3 diffuse  = diff * albedo * lightColor;
    vec3 specular = 0.15 * spec * lightColor;

    //vec3 color = ambient + diffuse + specular;

    //licurici
    vec3 sumLights = vec3(0);
    for (int i = 0; i < uFireflyCount; i++)
    {
        sumLights += PointLightPhong(
            uFireflyPos[i],
            uFireflyColor[i],
            uFireflyIntensity[i],
            FragPos,
            N,
            V,
            albedo
        );
    }
    //spotlights
    vec3 spotSum = vec3(0);
    for (int i = 0; i < uSpotCount; i++)
    {
        spotSum += SpotLightPhong(
            uSpotPos[i],
            uSpotDir[i],
            uSpotColor[i],
            uSpotCutoff[i],
            uSpotOuterCutoff[i],
            FragPos,
            N,
            V,
            albedo
        );
    }

    vec3 color = sumLights + spotSum;

    //ceata
    

    float d = length(viewPos - FragPos);
    
    // f = clamp(d / d_max, 0, 1)
    float fog_factor = clamp(d / max_distance, 0.0, 1.0);
    

    vec3 final_color = mix(color, fog_color, fog_factor);

    fragColor = vec4(final_color, 1.0);
}