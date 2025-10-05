#pragma once

#include "core/Engine/Mesh.h"
#include "core/math/mat.h"

namespace dem{

class Object{
public:
    dem::Mesh mesh;
    dem::math::mat4 projection;

    Object();
};
}