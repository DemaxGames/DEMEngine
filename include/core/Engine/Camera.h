#pragma once

#include "core/math/vec.h"
#include "core/math/mat.h"

namespace dem{

class Camera{
public:
    math::mat4 model_matrix;
    math::vec3 position;
    math::vec3 rotation;
    math::vec3 localRotation;

    math::vec3 forward;
    math::vec3 right;
    math::vec3 up;

    math::mat4 projection;
    float nearClip;
    float farClip;
    float fov;
    int width;
    int height;

    Camera();
    math::mat4 GetProjectionMatrix();
    math::mat4 GetModelMatrix();
};
}