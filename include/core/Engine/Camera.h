#pragma once

#include "core/math/vec.h"
#include "core/math/mat.h"

namespace dem{

class Camera{
public:
    math::mat4 projection;

    Camera();
};
}