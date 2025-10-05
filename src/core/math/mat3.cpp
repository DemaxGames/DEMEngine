#include "core/math/mat.h"

dem::math::mat3::mat3(dem::math::vec3 a, dem::math::vec3 b, dem::math::vec3 c) : v1(a), v2(b), v3(c){}

dem::math::mat3::mat3(float f11, float f12, float f13, 
        float f21, float f22, float f23, 
        float f31, float f32, float f33){
    v1[0] = f11;
    v2[0] = f12;
    v3[0] = f13;
    v1[1] = f21;
    v2[1] = f22;
    v3[1] = f23;
    v1[2] = f31;
    v2[2] = f32;
    v3[2] = f33;
}

void dem::math::mat3::identity(){
    *this = dem::math::mat3(1.f, 0.f, 0.f,
                            0.f, 1.f, 0.f,
                            0.f, 0.f, 1.f);
}

dem::math::mat3 dem::math::mat3::operator*(const dem::math::mat3& b){
    dem::math::mat3 c(v1[0] * b.v1[0] + v2[0] * b.v1[1] + v3[0] * b.v1[2], v1[0] * b.v2[0] + v2[0] * b.v2[1] + v3[0] * b.v2[2], v1[0] * b.v3[0] + v2[0] * b.v3[1] + v3[0] * b.v3[2],
                      v1[1] * b.v1[0] + v2[1] * b.v1[1] + v3[1] * b.v1[2], v1[1] * b.v2[0] + v2[1] * b.v2[1] + v3[1] * b.v2[2], v1[1] * b.v3[0] + v2[1] * b.v3[1] + v3[1] * b.v3[2],
                      v1[2] * b.v1[0] + v2[2] * b.v1[1] + v3[2] * b.v1[2], v1[2] * b.v2[0] + v2[2] * b.v2[1] + v3[2] * b.v2[2], v1[2] * b.v3[0] + v2[2] * b.v3[1] + v3[2] * b.v3[2]);
    return c;
}

dem::math::vec3 dem::math::mat3::operator*(const dem::math::vec3& b){
    return v1 * b[0] + v2 * b[1] + v3 * b[3];
}

dem::math::mat3& dem::math::mat3::operator=(const dem::math::mat3& b){
    v1 = b[0];
    v2 = b[1];
    v3 = b[2];
    return *this;
}


dem::math::vec3& dem::math::mat3::operator[](size_t index){
    return data[index];
}

const dem::math::vec3& dem::math::mat3::operator[](size_t index) const{
    return data[index];
}
