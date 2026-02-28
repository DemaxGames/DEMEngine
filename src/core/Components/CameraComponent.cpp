#include "core/Components/CameraComponent.h"

#include <cmath>

namespace dem{

CameraComponent::CameraComponent(){
    nearClip = 0.1f;
    farClip = 1000.f;
    fov = 3.1415926535f/2.f;
    width = 1280;
    height = 720;    
}

math::mat4 CameraComponent::GetProjectionMatrix(){
    math::mat4 result;
    result.projection((float)width/height, fov, nearClip, farClip);
    return result;
}

}