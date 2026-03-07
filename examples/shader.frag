#version 330 core

in vec3 n;
in vec2 fragUV;

uniform sampler2D tex;

void main(){
    gl_FragColor = vec4(texture2D(tex, fragUV));
    //gl_FragColor = vec4(normalize(abs(n)), 1.0);
}