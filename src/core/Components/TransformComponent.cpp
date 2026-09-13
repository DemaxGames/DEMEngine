#include "core/Components/TransformComponent.h"
#include <cmath>
#include "core/Logger/Logger.h"


namespace dem{

TransformComponent::TransformComponent(){
    position = math::vec3(0.f, 0.f, 0.f);
    rotation = math::vec3(0.f, 0.f, 0.f);
    scale = math::vec3(1.f, 1.f, 1.f);

    parent = nullptr;
}

math::mat4 TransformComponent::GetModelMatrix(){
    math::mat4 scaleMat;
    math::mat4 rotationMat;
    math::mat4 positionMat;

    scaleMat.scale(scale);
    rotationMat.rotation(rotation);
    positionMat.position(position);

    math::mat4 modelMat = positionMat * rotationMat * scaleMat;

    if(parent != nullptr) modelMat = modelMat * parent->GetModelMatrix();   
                                      
    forward = math::normalize(math::vec3()-(math::vec3)modelMat[2]);
    up = math::normalize((math::vec3)modelMat[1]);
    right = math::normalize((math::vec3)modelMat[0]);

    // forward = math::vec3(cos(rotation[0]) * sin(rotation[1]), -sin(rotation[0]), cos(rotation[0]) * cos(rotation[1]));
    // up = math::vec3(cos(rotation[1]), 0, -sin(rotation[1]));
    // right = math::vec3(sin(rotation[0]) * sin(rotation[1]), cos(rotation[0]), sin(rotation[0]) * cos(rotation[1]));

    return modelMat;
}

math::vec3 TransformComponent::GetWorldPosition(){
    math::vec4 a = math::vec4(position, 1.0);
    if(parent != nullptr) a = parent->GetModelMatrix() * a;
    if(a[3] == 0.0) return (math::vec3)a;
    return (math::vec3)a / a[3];
}

math::mat4 TransformComponent::GetRotationMatrix(){
    math::mat4 local_rotation_mat;
    local_rotation_mat.rotation(rotation);
    if(parent != nullptr){
        local_rotation_mat = parent->GetRotationMatrix() * local_rotation_mat;
    }
    return local_rotation_mat;
}

}