#include "core/math/vec.h"

namespace dem{

math::vec3::vec3(){
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
}

math::vec3::vec3(float a, float b, float c){
    data[0] = a;
    data[1] = b;
    data[2] = c;
}


math::vec3 math::vec3::operator-(const math::vec3& b){
    math::vec3 c(data[0] - b.data[0], data[1] - b.data[1], data[2] - b.data[2]);
    return c;
}

math::vec3 math::vec3::operator+(const math::vec3& b){
    math::vec3 c(data[0] + b.data[0], data[1] + b.data[1], data[2] + b.data[2]);
    return c;
}

math::vec3& math::vec3::operator=(const math::vec3& b){
    data[0] = b.data[0];
    data[1] = b.data[1];
    data[2] = b.data[2];
    return *this;
}

math::vec3 math::vec3::operator*(const float b){
    math::vec3 c(data[0] * b, data[1] * b, data[2] * b);
    return c;
}

math::vec3 math::vec3::operator/(const float b){
    math::vec3 c(data[0] / b, data[1] / b, data[2] / b);
    return c;
}


math::vec3& math::vec3::operator+=(const math::vec3& b){
    data[0] += b.data[0];
    data[1] += b.data[1];
    data[2] += b.data[2];
    return *this;
}

math::vec3& math::vec3::operator-=(const math::vec3& b){
    data[0] -= b.data[0];
    data[1] -= b.data[1];
    data[2] -= b.data[2];
    return *this;
}

math::vec3& math::vec3::operator*=(const float b){
    data[0] *= b;
    data[1] *= b;
    data[2] *= b;
    return *this;
}

math::vec3& math::vec3::operator/=(const float b){
    data[0] /= b;
    data[1] /= b;
    data[2] /= b;
    return *this;
}

float& math::vec3::operator[](size_t index){
    return data[index];
}

const float& math::vec3::operator[](size_t index) const{
    return data[index];
}

}