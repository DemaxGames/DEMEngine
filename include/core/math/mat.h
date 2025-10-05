#pragma once

#include <cstddef>

#include "core/math/vec.h"

namespace dem{
namespace math{

class mat2{
public:
    vec2 data[2];
    vec2& v1 = data[0];
    vec2& v2 = data[1];

    mat2(vec2 a, vec2 b);
    mat2(float f11, float f12, 
        float f21, float f22);

    void identity();
    
    mat2 operator*(const mat2& b);
    vec2 operator*(const vec2& b);
    mat2& operator=(const mat2& b);

    vec2& operator[](size_t index);
    const vec2& operator[](size_t index) const;
};

class mat3{
public:
    vec3 data[3];
    vec3& v1 = data[0];
    vec3& v2 = data[1];
    vec3& v3 = data[2];
    
    mat3(vec3 a, vec3 b, vec3 c);
    mat3(float f11, float f12, float f13, 
        float f21, float f22, float f23, 
        float f31, float f32, float f33);

    void identity();
    
    mat3 operator*(const mat3& b);
    vec3 operator*(const vec3& b);
    mat3& operator=(const mat3& b);

    vec3& operator[](size_t index);
    const vec3& operator[](size_t index) const;
};

class mat4{
public:
    vec4 data[4];
    vec4& v1 = data[0];
    vec4& v2 = data[1];
    vec4& v3 = data[2];
    vec4& v4 = data[3];

    mat4(vec4 a, vec4 b, vec4 c, vec4 d);
    mat4(float f11 = 0, float f12 = 0, float f13 = 0, float f14 = 0,
         float f21 = 0, float f22 = 0, float f23 = 0, float f24 = 0,
         float f31 = 0, float f32 = 0, float f33 = 0, float f34 = 0,
         float f41 = 0, float f42 = 0, float f43 = 0, float f44 = 0);

    void identity();
    void projection(float aspect, float fov, float near, float far);

    mat4 operator*(const mat4& b);
    vec4 operator*(const vec4& b);
    mat4& operator=(const mat4& b);

    vec4& operator[](size_t index);
    const vec4& operator[](size_t index) const;
};

}
}