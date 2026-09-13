#version 330 core
in vec2 vPos;

uniform vec3 textureUV[100];
uniform vec2 offsets[100];

out vec2 fPos;
out vec3 fUV;

void main()
{     
    fPos = vPos;
    fUV = textureUV[gl_InstanceID];
    vec2 offset = offsets[gl_InstanceID];
    gl_Position = vec4((vPos.x * fUV.z + offset.x) * 2.0, (vPos.y + offset.y) / 18.0, 0.0, 1.0);
}