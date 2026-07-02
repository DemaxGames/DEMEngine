#version 330 core

in vec2 fPos;
flat in int id;

uniform sampler2D tex;

out vec4 FragColor;

void main()
{
    FragColor = texture(tex, (fPos * 3.0) + vec2(float(id) / 10, 0.1));
}