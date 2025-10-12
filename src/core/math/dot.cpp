#include "core/math/vec.h"

namespace dem{

float math::dot(vec2 a, vec2 b){
    return a[0] * b[0] + a[1] * b[1];
}

float math::dot(vec3 a, vec3 b){
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

float math::dot(vec4 a, vec4 b){
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

    
}