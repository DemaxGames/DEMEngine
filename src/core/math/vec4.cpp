#include "core/math/vec.h"

namespace dem{

math::vec4::vec4(){
    X = 0;
    Y = 0;
    Z = 0;
    W = 0;
}

math::vec4::vec4(float a, float b, float c, float d){
    X = a;
    Y = b;
    Z = c;
    W = d;
}


math::vec4 math::vec4::operator-(const math::vec4& b){
    math::vec4 c(X - b.X, Y - b.Y, Z - b.Z, W - b.W);
    return c;
}

math::vec4 math::vec4::operator+(const math::vec4& b){
    math::vec4 c(X + b.X, Y + b.Y, Z + b.Z, W - b.W);
    return c;
}

math::vec4& math::vec4::operator=(const math::vec4& b){
    X = b.X;
    Y = b.Y;
    Z = b.Z;
    W = b.W;
    return *this;
}

math::vec4 math::vec4::operator*(const float b){
    math::vec4 c(X * b, Y * b, Z * b, W * b);
    return c;
}

math::vec4 math::vec4::operator/(const float b){
    math::vec4 c(X / b, Y / b, Z / b, W / b);
    return c;
}


math::vec4& math::vec4::operator+=(const math::vec4& b){
    X += b.X;
    Y += b.Y;
    Z += b.Z;
    W += b.W;
    return *this;
}

math::vec4& math::vec4::operator-=(const math::vec4& b){
    X -= b.X;
    Y -= b.Y;
    Z -= b.Z;
    W -= b.W;
    return *this;
}

math::vec4& math::vec4::operator*=(const float b){
    X *= b;
    Y *= b;
    Z *= b;
    W *= b;
    return *this;
}

math::vec4& math::vec4::operator/=(const float b){
    X /= b;
    Y /= b;
    Z /= b;
    W /= b;
    return *this;
}

float& math::vec4::operator[](size_t index){
    return data[index];
}

const float& math::vec4::operator[](size_t index) const{
    return data[index];
}
}