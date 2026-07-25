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
    float ambientStrength = 0.1;
    vec3 ambientColor = vec3(1.0, 1.0, 1.0);
    vec3 objectColor = texture(tex, fragUV).xyz;
    vec3 ambient = ambientStrength * ambientColor;

    vec3 norm = normalize(n);
    vec3 lightDir = normalize(lightPos - fPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.6;
    vec3 viewDir = normalize(viewPos - fPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    // vec3 result = (specular) * objectColor;
    FragColor = vec4(result, 1.0);
}