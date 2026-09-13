#pragma once

#include "core/math/vec.h"
#include "core/math/mat.h"
#include "core/Components/TransformComponent.h"

namespace dem{

enum Projection{
    Orthographic,
    Perspective
};

class CameraComponent{
public:
    TransformComponent *pTransform;
    float nearClip;
    float farClip;
    float fov;
    int width;
    int height;
    float size;
    Projection proj;

    CameraComponent();
    math::mat4 GetProjectionMatrix();
    math::mat4 GetViewMatrix();
};
}