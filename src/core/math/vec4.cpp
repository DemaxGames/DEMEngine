#include "core/math/vec.h"

namespace dem{

math::vec4::vec4(){
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;
}

math::vec4::vec4(float a, float b, float c, float d){
    data[0] = a;
    data[1] = b;
    data[2] = c;
    data[3] = d;
}

math::vec4::vec4(math::vec3 v, float d){
    data[0] = v[0];
    data[1] = v[1];
    data[2] = v[2];
    data[3] = d;
}


math::vec4 math::vec4::operator-(const math::vec4& b){
    math::vec4 c(data[0] - b.data[0], data[1] - b.data[1], data[2] - b.data[2], data[3] - b.data[3]);
    return c;
}

math::vec4 math::vec4::operator+(const math::vec4& b){
    math::vec4 c(data[0] + b.data[0], data[1] + b.data[1], data[2] + b.data[2], data[3] + b.data[3]);
    return c;
}

math::vec4& math::vec4::operator=(const math::vec4& b){
    data[0] = b.data[0];
    data[1] = b.data[1];
    data[2] = b.data[2];
    data[3] = b.data[3];
    return *this;
}

math::vec4 math::vec4::operator*(const float b){
    math::vec4 c(data[0] * b, data[1] * b, data[2] * b, data[3] * b);
    return c;
}

math::vec4 math::vec4::operator/(const float b){
    math::vec4 c(data[0] / b, data[1] / b, data[2] / b, data[3] / b);
    return c;
}


math::vec4& math::vec4::operator+=(const math::vec4& b){
    data[0] += b.data[0];
    data[1] += b.data[1];
    data[2] += b.data[2];
    data[3] += b.data[3];
    return *this;
}

math::vec4& math::vec4::operator-=(const math::vec4& b){
    data[0] -= b.data[0];
    data[1] -= b.data[1];
    data[2] -= b.data[2];
    data[3] -= b.data[3];
    return *this;
}

math::vec4& math::vec4::operator*=(const float b){
    data[0] *= b;
    data[1] *= b;
    data[2] *= b;
    data[3] *= b;
    return *this;
}

math::vec4& math::vec4::operator/=(const float b){
    data[0] /= b;
    data[1] /= b;
    data[2] /= b;
    data[3] /= b;
    return *this;
}

math::vec4::operator math::vec3() const{
    return math::vec3(data[0], data[1], data[2]);
}

bool math::vec4::operator==(const math::vec4&b){
    return this->data[0] == b.data[0] && this->data[1] == b.data[1] && this->data[2] == b.data[2] && this->data[3] == b.data[3];
}


bool math::vec4::operator!=(const math::vec4&b){
    return !(*this == b);
}

float& math::vec4::operator[](size_t index){
    return data[index];
}

const float& math::vec4::operator[](size_t index) const{
    return data[index];
}
}