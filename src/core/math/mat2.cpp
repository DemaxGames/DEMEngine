#include "core/math/mat.h"
#include "core/Logger/Logger.h"

namespace dem{

math::mat2::mat2(math::vec2 a, math::vec2 b) : v1(a), v2(b){}

math::mat2::mat2(float f11 = 0, float f12 = 0, 
        float f21 = 0, float f22 = 0 ){
    v1[0] = f11;
    v2[0] = f12;
    v1[1] = f21;
    v2[1] = f22;
}

void math::mat2::identity(){
    v1[0] = 1;
    v2[1] = 1;
}

math::mat2 math::mat2::operator*(const math::mat2& b){
    math::mat2 c(v1[0] * b.v1[0] + v2[0] * b.v1[1], v1[0] * b.v2[0] + v2[0] * b.v2[1],
                      v1[1] * b.v1[0] + v2[1] * b.v1[1], v1[1] * b.v2[0] + v2[1] * b.v2[1]);
    return c;
}

math::vec2 math::mat2::operator*(const math::vec2& b){
    return v1 * b[0] + v2 * b[1];
}

math::mat2& math::mat2::operator=(const math::mat2& b){
    v1 = b[0];
    v2 = b[1];
    return *this;
}


math::vec2& math::mat2::operator[](size_t index){
    return data[index];
}

const math::vec2& math::mat2::operator[](size_t index) const{
    return data[index];
}

}