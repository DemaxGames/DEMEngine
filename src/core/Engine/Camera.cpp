#include "core/Engine/Camera.h"


#include <cmath>

namespace dem{

Camera::Camera(){
    projection = math::mat4();

    nearClip = 0.1f;
    farClip = 1000.f;
    fov = 3.1415926535f/2.f;
    width = 1280;
    height = 720;

    math::vec3 position = math::vec3(0.0f, 0.0f, 0.0f);
    math::vec3 rotation = math::vec3(0.0f, 0.0f, 0.0f);
    math::vec3 localRotation = math::vec3(0.0f, 0.0f, 0.0f);
    
    model_matrix.identity();
    projection.projection((float)width/height, fov, nearClip, farClip);
}

math::mat4 Camera::GetProjectionMatrix(){
    projection.projection((float)width/height, fov, nearClip, farClip);
    return projection;
}

math::mat4 Camera::GetModelMatrix(){
    model_matrix = math::mat4(std::cos(rotation[1])*std::cos(rotation[2]), std::sin(rotation[0])*std::sin(rotation[1])*std::cos(rotation[2]) - std::cos(rotation[0])*std::sin(rotation[2]), std::cos(rotation[0])*std::sin(rotation[1])*std::cos(rotation[2]) + std::sin(rotation[0])*std::sin(rotation[2]), 0.f,
                              std::cos(rotation[1])*std::sin(rotation[2]), std::sin(rotation[0])*std::sin(rotation[1])*std::sin(rotation[2]) + std::cos(rotation[0])*std::cos(rotation[2]), std::cos(rotation[0])*std::sin(rotation[1])*std::sin(rotation[2]) - std::sin(rotation[0])*std::cos(rotation[2]), 0.f,
                              -std::sin(rotation[1]), std::sin(rotation[0])*std::cos(rotation[1]), std::cos(rotation[0])*std::cos(rotation[1]), 0.f,
                              0.f, 0.f, 0.f, 1.f);

    forward = math::normalize(math::vec3(-model_matrix[2][0], model_matrix[2][1], model_matrix[2][2]));
    up = math::normalize(math::vec3(-model_matrix[1][0], model_matrix[1][1], model_matrix[1][2]));
    right = math::normalize(math::vec3(-model_matrix[0][0], model_matrix[0][1], model_matrix[0][2]));
    
    model_matrix = math::mat4(std::cos(localRotation[1])*std::cos(localRotation[2]), std::sin(localRotation[0])*std::sin(localRotation[1])*std::cos(localRotation[2]) - std::cos(localRotation[0])*std::sin(localRotation[2]), std::cos(localRotation[0])*std::sin(localRotation[1])*std::cos(localRotation[2]) + std::sin(localRotation[0])*std::sin(localRotation[2]), 0.f,
    std::cos(localRotation[1])*std::sin(localRotation[2]), std::sin(localRotation[0])*std::sin(localRotation[1])*std::sin(localRotation[2]) + std::cos(localRotation[0])*std::cos(localRotation[2]), std::cos(localRotation[0])*std::sin(localRotation[1])*std::sin(localRotation[2]) - std::sin(localRotation[0])*std::cos(localRotation[2]), 0.f,
    -std::sin(localRotation[1]), std::sin(localRotation[0])*std::cos(localRotation[1]), std::cos(localRotation[0])*std::cos(localRotation[1]), 0.f,
    0.f, 0.f, 0.f, 1.f) * model_matrix;
    
    model_matrix = model_matrix * math::mat4(1.f, 0.f, 0.f, position[0],
                                             0.f, 1.f, 0.f, position[1],
                                             0.f, 0.f, 1.f, position[2],
                                            0.f, 0.f, 0.f, 1.f);
                                            
    return model_matrix;
}

}