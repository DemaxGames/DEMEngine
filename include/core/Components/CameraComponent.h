#pragma once

#include "core/math/vec.h"
#include "core/math/mat.h"

namespace dem{

enum Projection{
    Orthographic,
    Perspective
};

class CameraComponent{
public:
    float nearClip;
    float farClip;
    float fov;
    int width;
    int height;
    float size;
    Projection proj;

    CameraComponent();
    math::mat4 GetProjectionMatrix();
};
}