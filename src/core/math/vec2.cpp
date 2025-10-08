#include "core/math/vec.h"

namespace dem{

math::vec2::vec2(){
    X = 0;
    Y = 0;
}

math::vec2::vec2(float a, float b){
    X = a;
    Y = b;
}


math::vec2 math::vec2::operator-(const math::vec2& b){
    math::vec2 c(X - b.X, Y - b.Y);
    return c;
}

math::vec2 math::vec2::operator+(const math::vec2& b){
    math::vec2 c(X + b.X, Y + b.Y);
    return c;
}

math::vec2& math::vec2::operator=(const math::vec2& b){
    X = b.X;
    Y = b.Y;
    return *this;
}

math::vec2 math::vec2::operator*(const float b){
    math::vec2 c(X * b, Y * b);
    return c;
}

math::vec2 math::vec2::operator/(const float b){
    math::vec2 c(X / b, Y / b);
    return c;
}


math::vec2& math::vec2::operator+=(const math::vec2& b){
    X += b.X;
    Y += b.Y;
    return *this;
}

math::vec2& math::vec2::operator-=(const math::vec2& b){
    X -= b.X;
    Y -= b.Y;
    return *this;
}

math::vec2& math::vec2::operator*=(const float b){
    X *= b;
    Y *= b;
    return *this;
}

math::vec2& math::vec2::operator/=(const float b){
    X /= b;
    Y /= b;
    return *this;
}

float& math::vec2::operator[] (size_t index){
    return data[index];
}

const float& math::vec2::operator[] (size_t index) const{
    return data[index];
}
}