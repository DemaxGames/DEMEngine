#version 330 core

in vec3 vPos;
in vec3 normal;
in vec2 uv;

out vec3 n;
out vec2 fragUV;
out vec3 fPos;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main(){
    gl_Position = projection * view * model * vec4(vPos, 1.0);
    n = mat3(transpose(inverse(model))) * normal;
    fragUV = uv;
    fPos = (model * vec4(vPos, 1.0)).xyz;
}