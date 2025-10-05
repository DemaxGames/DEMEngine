#include "core/math/vec.h"

dem::math::vec3::vec3(){
    X = 0;
    Y = 0;
    Z = 0;
}

dem::math::vec3::vec3(float a, float b, float c){
    X = a;
    Y = b;
    Z = c;
}


dem::math::vec3 dem::math::vec3::operator-(const dem::math::vec3& b){
    dem::math::vec3 c(X - b.X, Y - b.Y, Z - b.Z);
    return c;
}

dem::math::vec3 dem::math::vec3::operator+(const dem::math::vec3& b){
    dem::math::vec3 c(X + b.X, Y + b.Y, Z + b.Z);
    return c;
}

dem::math::vec3& dem::math::vec3::operator=(const dem::math::vec3& b){
    X = b.X;
    Y = b.Y;
    Z = b.Z;
    return *this;
}

dem::math::vec3 dem::math::vec3::operator*(const float b){
    dem::math::vec3 c(X * b, Y * b, Z * b);
    return c;
}

dem::math::vec3 dem::math::vec3::operator/(const float b){
    dem::math::vec3 c(X / b, Y / b, Z / b);
    return c;
}


dem::math::vec3& dem::math::vec3::operator+=(const dem::math::vec3& b){
    X += b.X;
    Y += b.Y;
    Z += b.Z;
    return *this;
}

dem::math::vec3& dem::math::vec3::operator-=(const dem::math::vec3& b){
    X -= b.X;
    Y -= b.Y;
    Z -= b.Z;
    return *this;
}

dem::math::vec3& dem::math::vec3::operator*=(const float b){
    X *= b;
    Y *= b;
    Z *= b;
    return *this;
}

dem::math::vec3& dem::math::vec3::operator/=(const float b){
    X /= b;
    Y /= b;
    Z /= b;
    return *this;
}

float& dem::math::vec3::operator[](size_t index){
    return data[index];
}

const float& dem::math::vec3::operator[](size_t index) const{
    return data[index];
}

