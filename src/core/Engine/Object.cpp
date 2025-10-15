#include "core/Engine/Object.h"

#include <cmath>
#include "core/Logger/Logger.h"

namespace dem{

Object::Object(Mesh* pMesh){
    mesh = pMesh;
    math::vec3 position = math::vec3(0.0f, 0.0f, 0.0f);
    math::vec3 rotation = math::vec3(0.0f, 0.0f, 0.0f);
    math::vec3 scale = math::vec3(1.0f, 1.0f, 1.0f);
    model_matrix = math::mat4(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f);
    model_matrix.identity();
}

math::mat4 Object::GetModelMatrix(){
    model_matrix = math::mat4(std::cos(rotation[1])*std::cos(rotation[2]), std::sin(rotation[0])*std::sin(rotation[1])*std::cos(rotation[2]) - std::cos(rotation[0])*std::sin(rotation[2]), std::cos(rotation[0])*std::sin(rotation[1])*std::cos(rotation[2]) + std::sin(rotation[0])*std::sin(rotation[2]), position[0],
                              std::cos(rotation[1])*std::sin(rotation[2]), std::sin(rotation[0])*std::sin(rotation[1])*std::sin(rotation[2]) + std::cos(rotation[0])*std::cos(rotation[2]), std::cos(rotation[0])*std::sin(rotation[1])*std::sin(rotation[2]) - std::sin(rotation[0])*std::cos(rotation[2]), position[1],
                              -std::sin(rotation[1]), std::sin(rotation[0])*std::cos(rotation[1]), std::cos(rotation[0])*std::cos(rotation[1]), position[2],
                              0.f, 0.f, 0.f, 1.f);
    
    return model_matrix;
}

}
