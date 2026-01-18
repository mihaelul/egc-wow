#version 330 core

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Output c?tre Fragment Shader
out vec2 UV;
out vec3 FragPos;
out vec3 Normal;

void main()
{
    vec4 worldPos = Model * vec4(v_position, 1.0);

    FragPos = worldPos.xyz;
    UV = v_texture_coord;

    // transform normal to world space
    Normal = normalize(mat3(transpose(inverse(Model))) * v_normal);

    gl_Position = Projection * View * worldPos;
}
