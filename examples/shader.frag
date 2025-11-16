#version 330 core

in vec3 n;
in mat4 fragView;
in vec2 fragPos;

uniform sampler2D tex;

void main(){
    //gl_FragColor = vec4(normalize(abs(n)), 1.0);
    gl_FragColor = texture2D(tex, fragPos);
}