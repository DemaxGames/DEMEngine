#version 330 core

in vec3 pos;

void main(){
    gl_FragColor = vec4(abs(pos) * 10, 1.0);
}