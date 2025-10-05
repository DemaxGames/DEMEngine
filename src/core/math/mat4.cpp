#include "core/math/mat.h"

#include <cmath>

dem::math::mat4::mat4(dem::math::vec4 a, dem::math::vec4 b, dem::math::vec4 c, dem::math::vec4 d) : v1(a), v2(b), v3(c), v4(d){}

dem::math::mat4::mat4(float f11, float f12, float f13, float f14,
                      float f21, float f22, float f23, float f24,
                      float f31, float f32, float f33, float f34,
                      float f41, float f42, float f43, float f44){
    v1[0] = f11;
    v2[0] = f12;
    v3[0] = f13;
    v4[0] = f14;
    v1[1] = f21;
    v2[1] = f22;
    v3[1] = f23;
    v4[1] = f24;
    v1[2] = f31;
    v2[2] = f32;
    v3[2] = f33;
    v4[2] = f34;
    v1[3] = f41;
    v2[3] = f42;
    v3[3] = f43;
    v4[3] = f44;
}

void dem::math::mat4::identity(){
    *this = dem::math::mat4(1.f, 0.f, 0.f, 0.f,
                            0.f, 1.f, 0.f, 0.f,
                            0.f, 0.f, 1.f, 0.f,
                            0.f, 0.f, 0.f, 1.f);
}

void dem::math::mat4::projection(float aspect, float fov, float near, float far){
    *this = dem::math::mat4(1/(aspect * std::tan(fov/2)), 0.f, 0.f, 0.f,
                            0.f, 1/(std::tan(fov/2)), 0.f, 0.f,
                            0.f, 0.f, -(far+near)/(far-near), -(2*far*near)/(far-near),
                            0.f, 0.f, -1.f, 0.f);
}


dem::math::mat4 dem::math::mat4::operator*(const dem::math::mat4& b){
    dem::math::mat4 c(v1[0] * b.v1[0] + v2[0] * b.v1[1] + v3[0] * b.v1[2] + v4[0] * b.v1[3], v1[0] * b.v2[0] + v2[0] * b.v2[1] + v3[0] * b.v2[2] + v4[0] * b.v2[3], v1[0] * b.v3[0] + v2[0] * b.v3[1] + v3[0] * b.v3[2] + v4[0] * b.v3[3], v1[0] * b.v4[0] + v2[0] * b.v4[1] + v3[0] * b.v4[2] + v4[0] * b.v4[3],
                      v1[1] * b.v1[0] + v2[1] * b.v1[1] + v3[1] * b.v1[2] + v4[1] * b.v1[3], v1[1] * b.v2[0] + v2[1] * b.v2[1] + v3[1] * b.v2[2] + v4[1] * b.v2[3], v1[1] * b.v3[0] + v2[1] * b.v3[1] + v3[1] * b.v3[2] + v4[1] * b.v3[3], v1[1] * b.v4[0] + v2[1] * b.v4[1] + v3[1] * b.v4[2] + v4[1] * b.v4[3],
                      v1[2] * b.v1[0] + v2[2] * b.v1[1] + v3[2] * b.v1[2] + v4[2] * b.v1[3], v1[2] * b.v2[0] + v2[2] * b.v2[1] + v3[2] * b.v2[2] + v4[2] * b.v2[3], v1[2] * b.v3[0] + v2[2] * b.v3[1] + v3[2] * b.v3[2] + v4[2] * b.v3[3], v1[2] * b.v4[0] + v2[2] * b.v4[1] + v3[2] * b.v4[2] + v4[2] * b.v4[3],
                      v1[3] * b.v1[0] + v2[3] * b.v1[1] + v3[3] * b.v1[2] + v4[3] * b.v1[3], v1[3] * b.v2[0] + v2[3] * b.v2[1] + v3[3] * b.v2[2] + v4[3] * b.v2[3], v1[3] * b.v3[0] + v2[3] * b.v3[1] + v3[3] * b.v3[2] + v4[3] * b.v3[3], v1[3] * b.v4[0] + v2[3] * b.v4[1] + v3[3] * b.v4[2] + v4[3] * b.v4[3]);
    return c;
}

dem::math::vec4 dem::math::mat4::operator*(const dem::math::vec4& b){
    return v1 * b[0] + v2 * b[1] + v3 * b[2] + v4 * b[3];
}

dem::math::mat4& dem::math::mat4::operator=(const dem::math::mat4& b){
    v1 = b[0];
    v2 = b[1];
    v3 = b[2];
    v4 = b[3];
    return *this;
}


dem::math::vec4& dem::math::mat4::operator[](size_t index){
    return data[index];
}

const dem::math::vec4& dem::math::mat4::operator[](size_t index) const{
    return data[index];
}

