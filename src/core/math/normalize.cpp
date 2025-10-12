#include "core/math/vec.h"
#include <cmath>

namespace dem{

math::vec2 math::normalize(math::vec2 a){
    float magnitude = std::sqrt(a[0]*a[0] + a[1]*a[1]);
    return math::vec2(a[0]/magnitude, a[1]/magnitude);
}

math::vec3 math::normalize(math::vec3 a){
    float magnitude = std::sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
    return math::vec3(a[0]/magnitude, a[1]/magnitude, a[2]/magnitude);
}

math::vec4 math::normalize(math::vec4 a){
    float magnitude = std::sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2] + a[3]*a[3]);
    return math::vec4(a[0]/magnitude, a[1]/magnitude, a[2]/magnitude, a[3]/magnitude);
}

}