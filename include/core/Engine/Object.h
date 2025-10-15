#pragma once

#include "core/Engine/Mesh.h"
#include "core/math/mat.h"

namespace dem{

class Object{
public:
    Mesh* mesh;
    math::mat4 model_matrix;
    math::vec3 position;
    math::vec3 rotation;
    math::vec3 scale;
    
    Object(Mesh* mesh = NULL);
    math::mat4 GetModelMatrix();
};
}