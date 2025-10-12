#version 330 core

in vec3 vPos;

out vec3 pos;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main(){
    gl_Position = projection * view * model * vec4(vPos.x, vPos.y, vPos.z, 1.0);
    pos = vPos;
}