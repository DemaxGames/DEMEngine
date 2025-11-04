#version 330 core

in vec3 n;
in mat4 fragView;

void main(){
    gl_FragColor = vec4(normalize(abs(n)), 1.0);
}