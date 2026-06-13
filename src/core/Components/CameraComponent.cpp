#include "core/Components/CameraComponent.h"

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

}