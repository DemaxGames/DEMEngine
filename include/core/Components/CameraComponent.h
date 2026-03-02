#pragma once

#include "core/math/vec.h"
#include "core/math/mat.h"

namespace dem{

class CameraComponent{
public:
    float nearClip;
    float farClip;
    float fov;
    int width;
    int height;

    CameraComponent();
    math::mat4 GetProjectionMatrix();
};
}