#include "core/math/vec.h"

namespace dem{

math::vec2::vec2(){
    data[0] = 0;
    data[1] = 0;
}

math::vec2::vec2(float a, float b){
    data[0] = a;
    data[1] = b;
}


math::vec2 math::vec2::operator-(const math::vec2& b){
    math::vec2 c(data[0] - b.data[0], data[1] - b.data[1]);
    return c;
}

math::vec2 math::vec2::operator+(const math::vec2& b){
    math::vec2 c(data[0] + b.data[0], data[1] + b.data[1]);
    return c;
}

math::vec2& math::vec2::operator=(const math::vec2& b){
    data[0] = b.data[0];
    data[1] = b.data[1];
    return *this;
}

math::vec2 math::vec2::operator*(const float b){
    math::vec2 c(data[0] * b, data[1] * b);
    return c;
}

math::vec2 math::vec2::operator/(const float b){
    math::vec2 c(data[0] / b, data[1] / b);
    return c;
}


math::vec2& math::vec2::operator+=(const math::vec2& b){
    data[0] += b.data[0];
    data[1] += b.data[1];
    return *this;
}

math::vec2& math::vec2::operator-=(const math::vec2& b){
    data[0] -= b.data[0];
    data[1] -= b.data[1];
    return *this;
}

math::vec2& math::vec2::operator*=(const float b){
    data[0] *= b;
    data[1] *= b;
    return *this;
}

math::vec2& math::vec2::operator/=(const float b){
    data[0] /= b;
    data[1] /= b;
    return *this;
}

float& math::vec2::operator[] (size_t index){
    return data[index];
}

const float& math::vec2::operator[] (size_t index) const{
    return data[index];
}
}