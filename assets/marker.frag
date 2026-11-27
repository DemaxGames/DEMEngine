#version 330 core

in vec3 n;
in vec2 fragUV;
in vec3 fPos;

out vec4 FragColor;

uniform sampler2D tex;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main(){
    vec4 objectColor = texture(tex, vec2(fragUV.x, -fragUV.y));

    FragColor = vec4(objectColor);
}