#include <iostream>
#include "core/Logger/Logger.h"
#include "core/Engine/Mesh.h"
#include "core/Engine/Object.h"
#include "core/Engine/Scene.h"
#include "core/Renderer/Render.h"
#include "core/InputSystem/Input.h"
#include "core/math/vec.h"
#include "core/math/mat.h"
#include "core/time/time.h"
#include "core/math/uid.h"
#include "core/ECS/ecs.h"
#include "core/ECS/Component.h"
#include "core/ECS/Entity.h"
#include <stdio.h>
#include <filesystem>
#include <vector>
#include <windows.h>

class Transform{
public:
    dem::math::vec3 position;
};

int main(){
    dem::Logger *logger = dem::Logger::get();
    dem::ecs::init();

    dem::ecs::RegisterComponent<Transform>();
    
    dem::ecs::Entity cube = dem::ecs::CreateEntity();
    logger->log("Entity created");
    cube.AddComponent<Transform>();
    logger->log("Component added");
    Transform *cubeTransform;
    if((cubeTransform = cube.GetComponent<Transform>()) == nullptr){
        logger->log("ERROR: cannot get component<Transform>");
        return -1;
    }
    logger->log("Got component");
    
    cubeTransform->position = dem::math::vec3(12.f, -0.02f, 0.f);
    logger->log("CubeTransfrom: ", cubeTransform->position);

    return 0;
}