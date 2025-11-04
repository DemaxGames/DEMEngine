#include "core/math/vec.h"
#include <cmath>

namespace dem{

math::vec3 math::normal(math::vec3 a, math::vec3 b){
    math::vec3 n;
    n[0] = a[1] * b[2] - a[2] * b[1];
    n[1] = a[2] * b[0] - a[0] * b[2];
    n[2] = a[0] * b[1] - a[1] * b[0];

    return n;
}

}