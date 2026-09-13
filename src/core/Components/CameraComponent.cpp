#include "core/Components/CameraComponent.h"
#include "core/Logger/Logger.h"
#include <cmath>

namespace dem{

CameraComponent::CameraComponent(){
    nearClip = 0.1f;
    farClip = 1000.f;
    fov = 3.1415926535f/2.f;
    width = 1280;
    height = 720;
    size = 1.0;
    proj = Projection::Perspective;
}

math::mat4 CameraComponent::GetProjectionMatrix(){
    math::mat4 result;
    switch(proj){
        case Projection::Orthographic:
            result.orthographic((float)width/height * size/2.0,(float)width/height * -size/2.0, size/2.0, -size/2.0, nearClip, farClip);
            break;
        case Projection::Perspective:
            result.perspective((float)width/height, fov, nearClip, farClip);
            break;
    }
    return result;
}

math::mat4 CameraComponent::GetViewMatrix(){
    if(pTransform == nullptr){
        Logger::get()->log("ERROR: cannot get view matrix from camera component, the transform is not set");
        return math::mat4();
    }

    math::vec3 invPos = pTransform->GetWorldPosition() * -1.0f;
    math::mat4 invPosMat;
    invPosMat.position(invPos);
    math::mat4 invRot = pTransform->GetRotationMatrix();
    invRot.transpose();

    math::mat4 viewMat;
    viewMat.identity();
    viewMat = viewMat * invRot;
    viewMat = viewMat * invPosMat;
    return viewMat;
}


}