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

// --- Uniforms noi pentru ceață ---
uniform float max_distance; // d_max
uniform vec3 fog_color;     // C_fog (ex: vec3(0.5, 0.5, 0.5))

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

    vec3 color = ambient + diffuse + specular;

    //ceata
    

    float d = length(viewPos - FragPos);
    
    // f = clamp(d / d_max, 0, 1)
    float fog_factor = clamp(d / max_distance, 0.0, 1.0);
    

    vec3 final_color = mix(color, fog_color, fog_factor);

    fragColor = vec4(final_color, 1.0);
}