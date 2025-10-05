#version 330 core

in vec3 vPos;

out vec3 pos;

uniform mat4 projection;

void main(){
    gl_Position = projection * vec4(vPos.x, vPos.y, vPos.z, 1.0);
    pos = vPos;
}