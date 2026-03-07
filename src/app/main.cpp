#include <iostream>
#include <stdio.h>
#include <filesystem>
#include <vector>
#include <windows.h>
#include "core/dem.h"
#include "core/time/time.h"
#include <cmath>

#define PI 3.141592f

int main(){
    dem::Logger *logger = dem::Logger::get();
    dem::ecs::init();
    
    dem::ecs::RegisterComponent<dem::TransformComponent>();
    dem::ecs::RegisterComponent<dem::CameraComponent>();
    dem::ecs::RegisterComponent<dem::MeshRenderer>();
    
    dem::Renderer::Init(1280, 720);
    logger->log("material");

    dem::Image texture;
    texture.Load("examples/texture.png");

    dem::Material mat{};
    logger->log("material done");
    mat.glImage.image = &texture;
    mat.Load();

    dem::ecs::Entity player = dem::ecs::CreateEntity();
    dem::TransformComponent* playerTransform = player.AddComponent<dem::TransformComponent>();
    playerTransform->rotation = dem::math::vec3(0.f, 0.f, 0.f);
    
    dem::ecs::Entity camera = dem::ecs::CreateEntity();
    logger->log("camera id:", (uint64_t)camera.id);
    dem::TransformComponent* cameraTransform = camera.AddComponent<dem::TransformComponent>();
    cameraTransform->parent = player.GetComponent<dem::TransformComponent>();
    dem::CameraComponent* cameraComponent = camera.AddComponent<dem::CameraComponent>();
    cameraComponent->fov = PI * 1.f / 2.f;

    dem::Mesh mesh;
    mesh.Load("examples\\cube.obj");

    dem::ecs::Entity entity = dem::ecs::CreateEntity();
    dem::MeshRenderer* entityRenderer = entity.AddComponent<dem::MeshRenderer>();
    dem::TransformComponent* entityTransform = entity.AddComponent<dem::TransformComponent>();
    entityRenderer->material = &mat;
    entityRenderer->mesh = &mesh;
    entityRenderer->Load();

    dem::Input::Init();
    dem::Input::sensivity = 3.f;

    logger->log("entity count: ", (uint32_t)dem::ecs::entities.size());

    for(int i = 0; i < dem::ecs::entities.size(); i++){
        logger->log("entity ", i);
        logger->log("   id: ", (uint64_t)dem::ecs::entities[i].id);
        dem::TransformComponent* transform = dem::ecs::entities[i].GetComponent<dem::TransformComponent>();
        logger->log("   transform ", (uint64_t)transform);
        logger->log("   parent: ", (uint64_t)transform->parent);
    }

    logger->log("Starting Rendering");

    float deltaTime = 0;
    int frameCounter = 0;

    float fpsDeltatime = 0;
    int fpsFrames = 160;
    while(!dem::Renderer::Render()){
        frameCounter++;

        
        cameraTransform->rotation += dem::math::vec3(1.5f, 0.f, 0.f) * dem::Input::GetAxisY() * deltaTime;
        playerTransform->rotation += dem::math::vec3(0.f, 1.5f, 0.f) * dem::Input::GetAxisX() * deltaTime;
        if((cameraTransform->rotation[0] + 1.5 * dem::Input::GetAxisY() * deltaTime) > (PI / 2.f)){
            cameraTransform->rotation[0] = (PI / 2.f);
        }
        else if((cameraTransform->rotation[0] + 1.5 * dem::Input::GetAxisY() * deltaTime) < -(PI / 2.f)){
            cameraTransform->rotation[0] = -(PI / 2.f);
        }

        if(dem::Input::GetKey(dem::KeyCode::W)) playerTransform->position += playerTransform->forward * 1.5f * deltaTime;
        if(dem::Input::GetKey(dem::KeyCode::S)) playerTransform->position += playerTransform->forward * -1.5f * deltaTime;
        if(dem::Input::GetKey(dem::KeyCode::A)) playerTransform->position += playerTransform->right * -1.5f * deltaTime;
        if(dem::Input::GetKey(dem::KeyCode::D)) playerTransform->position += playerTransform->right * 1.5f * deltaTime;

        if(dem::Input::GetKey(dem::KeyCode::E)) playerTransform->position -= dem::math::vec3(0.f, 0.5f * deltaTime, 0.f);
        if(dem::Input::GetKey(dem::KeyCode::Q)) playerTransform->position += dem::math::vec3(0.f, 0.5f * deltaTime, 0.f);

        //std::cout << "                                                                                                                              \n";
        std::cout << "                                                                                                                              \r";
        std::cout << "cameraPos: " << playerTransform->position[0] << " " << playerTransform->position[1] << " " <<  playerTransform->position[2] << "\r";
        //std::cout << "cameraRot: " << cameraTransform->rotation[0] << " " << cameraTransform->rotation[1] << " " <<  cameraTransform->rotation[2] << "\r";
        //entityTransform->rotation = entityTransform->rotation + dem::math::vec3(0.f, 0.f, (float) deltaTime * 1.2f);

        fpsDeltatime += deltaTime;
        fpsFrames;
        if(frameCounter % fpsFrames == 0){
            logger->log("fps: ", (int)(1. / (fpsDeltatime / (float)fpsFrames)));
            fpsDeltatime = 0;
        }
        
        dem::Input::Update();
        dem::Time::Update();
        deltaTime = dem::Time::GetDeltaTime();
    }

    logger->log("Total frames generated: ", frameCounter);


    return 0;
}