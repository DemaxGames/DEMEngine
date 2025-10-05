#include "core/math/vec.h"

dem::math::vec4::vec4(){
    X = 0;
    Y = 0;
    Z = 0;
    W = 0;
}

dem::math::vec4::vec4(float a, float b, float c, float d){
    X = a;
    Y = b;
    Z = c;
    W = d;
}


dem::math::vec4 dem::math::vec4::operator-(const dem::math::vec4& b){
    dem::math::vec4 c(X - b.X, Y - b.Y, Z - b.Z, W - b.W);
    return c;
}

dem::math::vec4 dem::math::vec4::operator+(const dem::math::vec4& b){
    dem::math::vec4 c(X + b.X, Y + b.Y, Z + b.Z, W - b.W);
    return c;
}

dem::math::vec4& dem::math::vec4::operator=(const dem::math::vec4& b){
    X = b.X;
    Y = b.Y;
    Z = b.Z;
    W = b.W;
    return *this;
}

dem::math::vec4 dem::math::vec4::operator*(const float b){
    dem::math::vec4 c(X * b, Y * b, Z * b, W * b);
    return c;
}

dem::math::vec4 dem::math::vec4::operator/(const float b){
    dem::math::vec4 c(X / b, Y / b, Z / b, W / b);
    return c;
}


dem::math::vec4& dem::math::vec4::operator+=(const dem::math::vec4& b){
    X += b.X;
    Y += b.Y;
    Z += b.Z;
    W += b.W;
    return *this;
}

dem::math::vec4& dem::math::vec4::operator-=(const dem::math::vec4& b){
    X -= b.X;
    Y -= b.Y;
    Z -= b.Z;
    W -= b.W;
    return *this;
}

dem::math::vec4& dem::math::vec4::operator*=(const float b){
    X *= b;
    Y *= b;
    Z *= b;
    W *= b;
    return *this;
}

dem::math::vec4& dem::math::vec4::operator/=(const float b){
    X /= b;
    Y /= b;
    Z /= b;
    W /= b;
    return *this;
}

float& dem::math::vec4::operator[](size_t index){
    return data[index];
}

const float& dem::math::vec4::operator[](size_t index) const{
    return data[index];
}
