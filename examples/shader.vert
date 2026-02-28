#version 330 core

in vec3 vPos;
in vec3 normal;

out vec3 n;
out mat4 fragView;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main(){
    gl_Position = projection * view * model * vec4(vPos, 1.0);
    n = normal;
    fragView = view;
}