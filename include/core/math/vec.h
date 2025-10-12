#pragma once

#include <cstddef>


namespace dem{
namespace math{
    
class vec2{
public:
    float data[2];
    
    vec2();
    vec2(float a, float b);

    vec2 operator-(const vec2& b);
    vec2 operator+(const vec2& b);
    vec2& operator=(const vec2& b);
    vec2 operator*(const float b);
    vec2 operator/(const float b);

    vec2& operator+=(const vec2& b);
    vec2& operator-=(const vec2& b);
    vec2& operator*=(const float b);
    vec2& operator/=(const float b);

    float& operator[](size_t index);
    const float& operator[](size_t index) const;
};

class vec3{
public:
    float data[3];

    vec3();
    vec3(float a, float b, float c);

    vec3 operator-(const vec3& b);
    vec3 operator+(const vec3& b);
    vec3& operator=(const vec3& b);
    vec3 operator*(const float b);
    vec3 operator/(const float b);

    vec3& operator+=(const vec3& b);
    vec3& operator-=(const vec3& b);
    vec3& operator*=(const float b);
    vec3& operator/=(const float b);

    float& operator[](size_t index);
    const float& operator[](size_t index) const;
};

class vec4{
public:
    float data[4];

    vec4();
    vec4(float a, float b, float c, float d);

    vec4 operator-(const vec4& b);
    vec4 operator+(const vec4& b);
    vec4& operator=(const vec4& b);
    vec4 operator*(const float b);
    vec4 operator/(const float b);

    vec4& operator+=(const vec4& b);
    vec4& operator-=(const vec4& b);
    vec4& operator*=(const float b);
    vec4& operator/=(const float b);

    float& operator[](size_t index);
    const float& operator[](size_t index) const;
};

float dot(vec2 a, vec2 b);
float dot(vec3 a, vec3 b);
float dot(vec4 a, vec4 b);

vec2 normalize(vec2 a);
vec3 normalize(vec3 a);
vec4 normalize(vec4 a);

}
}