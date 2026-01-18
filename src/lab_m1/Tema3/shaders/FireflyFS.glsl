#version 330 core

out vec4 fragColor;

uniform vec3 fireflyColor;
uniform float uTime;
uniform float phase;

void main()
{
    float pulse = 0.6 + 0.4 * sin(uTime * 4.0 + phase);
    vec3 col = fireflyColor * pulse;

    fragColor = vec4(col, 1.0);
}