#pragma once
#include "core/math/mat.h"

namespace dem{

class TransformComponent{
public:
    math::vec3 position;
    math::vec3 rotation;
    math::vec3 scale;

    math::vec3 forward;
    math::vec3 right;
    math::vec3 up;

    TransformComponent* parent;

    TransformComponent();

    math::mat4 GetModelMatrix();
};

}