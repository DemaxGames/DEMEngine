#version 330 core

in vec3 n;
in vec2 fragUV;

out vec4 FragColor;

uniform sampler2D tex;

void main(){
    float ambientStrength = 1.0;

    //vec3 norm = normalize(n);
    //vec3 lightDir = normalize(vec3(0.0, 0.0, 1.0));
    //float diff = max(dot(norm, lightDir), 0.0);

    vec4 color = texture(tex, fragUV);
    FragColor = color * min(ambientStrength, 1.0);
}