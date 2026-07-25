#pragma once

#include "core/Components/TransformComponent.h"
#include "core/math/vec.h"

namespace dem{
 
class LightSourceComponent{
public:
    TransformComponent* transform;
    math::vec3 color;
    float strength;

    LightSourceComponent();
};

}