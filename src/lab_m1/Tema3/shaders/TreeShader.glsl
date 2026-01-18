#version 330 core

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

out vec2 UV;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float TREE_HEIGHT;   // ex: 2.0

void main()
{
    vec3 pos = v_position;

    float y = pos.y + TREE_HEIGHT * 0.5;

    // t = y / TREE_HEIGHT
    float t = clamp(y / TREE_HEIGHT, 0.0, 1.0);

    //  scale = 1 - t
    float scale = 1.0 - t;


    pos.x *= scale;
    pos.z *= scale;

    vec4 worldPos = Model * vec4(pos, 1.0);

    FragPos = worldPos.xyz;
    UV = v_texture_coord;
    Normal = normalize(mat3(transpose(inverse(Model))) * v_normal);

    gl_Position = Projection * View * worldPos;
}
