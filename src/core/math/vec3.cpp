#include "core/math/vec.h"

namespace dem{

math::vec3::vec3(){
    X = 0;
    Y = 0;
    Z = 0;
}

math::vec3::vec3(float a, float b, float c){
    X = a;
    Y = b;
    Z = c;
}


math::vec3 math::vec3::operator-(const math::vec3& b){
    math::vec3 c(X - b.X, Y - b.Y, Z - b.Z);
    return c;
}

math::vec3 math::vec3::operator+(const math::vec3& b){
    math::vec3 c(X + b.X, Y + b.Y, Z + b.Z);
    return c;
}

math::vec3& math::vec3::operator=(const math::vec3& b){
    X = b.X;
    Y = b.Y;
    Z = b.Z;
    return *this;
}

math::vec3 math::vec3::operator*(const float b){
    math::vec3 c(X * b, Y * b, Z * b);
    return c;
}

math::vec3 math::vec3::operator/(const float b){
    math::vec3 c(X / b, Y / b, Z / b);
    return c;
}


math::vec3& math::vec3::operator+=(const math::vec3& b){
    X += b.X;
    Y += b.Y;
    Z += b.Z;
    return *this;
}

math::vec3& math::vec3::operator-=(const math::vec3& b){
    X -= b.X;
    Y -= b.Y;
    Z -= b.Z;
    return *this;
}

math::vec3& math::vec3::operator*=(const float b){
    X *= b;
    Y *= b;
    Z *= b;
    return *this;
}

math::vec3& math::vec3::operator/=(const float b){
    X /= b;
    Y /= b;
    Z /= b;
    return *this;
}

float& math::vec3::operator[](size_t index){
    return data[index];
}

const float& math::vec3::operator[](size_t index) const{
    return data[index];
}

}