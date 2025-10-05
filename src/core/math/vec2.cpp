#include "core/math/vec.h"

dem::math::vec2::vec2(){
    X = 0;
    Y = 0;
}

dem::math::vec2::vec2(float a, float b){
    X = a;
    Y = b;
}


dem::math::vec2 dem::math::vec2::operator-(const dem::math::vec2& b){
    dem::math::vec2 c(X - b.X, Y - b.Y);
    return c;
}

dem::math::vec2 dem::math::vec2::operator+(const dem::math::vec2& b){
    dem::math::vec2 c(X + b.X, Y + b.Y);
    return c;
}

dem::math::vec2& dem::math::vec2::operator=(const dem::math::vec2& b){
    X = b.X;
    Y = b.Y;
    return *this;
}

dem::math::vec2 dem::math::vec2::operator*(const float b){
    dem::math::vec2 c(X * b, Y * b);
    return c;
}

dem::math::vec2 dem::math::vec2::operator/(const float b){
    dem::math::vec2 c(X / b, Y / b);
    return c;
}


dem::math::vec2& dem::math::vec2::operator+=(const dem::math::vec2& b){
    X += b.X;
    Y += b.Y;
    return *this;
}

dem::math::vec2& dem::math::vec2::operator-=(const dem::math::vec2& b){
    X -= b.X;
    Y -= b.Y;
    return *this;
}

dem::math::vec2& dem::math::vec2::operator*=(const float b){
    X *= b;
    Y *= b;
    return *this;
}

dem::math::vec2& dem::math::vec2::operator/=(const float b){
    X /= b;
    Y /= b;
    return *this;
}

float& dem::math::vec2::operator[] (size_t index){
    return data[index];
}

const float& dem::math::vec2::operator[] (size_t index) const{
    return data[index];
}
