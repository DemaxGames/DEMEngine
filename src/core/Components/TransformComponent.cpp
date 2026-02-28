#include "core/Components/TransformComponent.h"
#include <cmath>


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
    math::mat4 rotationMatZ;
    math::mat4 rotationMatX;
    math::mat4 rotationMatY;
    math::mat4 positionMat;

    scaleMat.scale(scale);
    rotationMat.identity();
    rotationMatY.rotationZ(rotation[2]);
    rotationMatZ.rotationX(rotation[0]);
    rotationMatX.rotationY(rotation[1]);
    rotationMat = rotationMat * rotationMatZ;
    rotationMat = rotationMat * rotationMatX;
    rotationMat = rotationMat * rotationMatY;
    positionMat.position(position);

    math::mat4 modelMat = rotationMat * positionMat * scaleMat;

    if(parent != nullptr) modelMat = modelMat * parent->GetModelMatrix();
                                      
    forward = math::normalize(math::vec3(-modelMat[2][0], modelMat[2][1], modelMat[2][2]));
    up = math::normalize(math::vec3(-modelMat[1][0], modelMat[1][1], modelMat[1][2]));
    right = math::normalize(math::vec3(-modelMat[0][0], modelMat[0][1], modelMat[0][2]));

    // forward = math::vec3(cos(rotation[0]) * sin(rotation[1]), -sin(rotation[0]), cos(rotation[0]) * cos(rotation[1]));
    // up = math::vec3(cos(rotation[1]), 0, -sin(rotation[1]));
    // right = math::vec3(sin(rotation[0]) * sin(rotation[1]), cos(rotation[0]), sin(rotation[0]) * cos(rotation[1]));

    return modelMat;
}

}