#version 330 core

in vec2 fPos;
in vec3 fUV;

uniform sampler2D tex;
uniform vec3 color;

out vec4 FragColor;

void main()
{
    vec2 uv = vec2(fPos.x * fUV.z + fUV.x, -fPos.y);
    FragColor = vec4(color, texture(tex, uv).w);
}