#version 330 core

layout(location=0) in vec3 v_position;
layout(location=1) in vec3 v_normal;
layout(location=2) in vec2 v_texture_coord;

out vec2 UV;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
    vec4 worldPos = Model * vec4(v_position, 1.0);

    FragPos = worldPos.xyz;
    UV = v_texture_coord;

    Normal = normalize(mat3(transpose(inverse(Model))) * v_normal);

    gl_Position = Projection * View * worldPos;
}
