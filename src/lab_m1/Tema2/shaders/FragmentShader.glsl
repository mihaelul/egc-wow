#version 330

uniform vec3 objectColor;
layout(location = 0) out vec4 out_color;

void main()
{
    out_color = vec4(objectColor, 1.0);
}