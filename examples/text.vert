#version 330 core
in vec2 vPos;

uniform int data[100];
uniform vec2 offsets[100];

out vec2 fPos;
flat out int id;

void main()
{     
    fPos = vPos;
    id = data[gl_InstanceID];
    vec2 offset = offsets[gl_InstanceID];
    gl_Position = vec4(vPos + offset, 0.0, 1.0);
}