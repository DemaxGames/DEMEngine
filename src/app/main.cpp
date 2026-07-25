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

    dem::ecs::Entity entity = dem::ecs::CreateEntity();
    dem::TransformComponent* entityTransform = entity.AddComponent<dem::TransformComponent>();


    // std::cout << "my matrix: \n";

    // for(int x = 0; x < 4; x++){
    //     for(int y = 0; y < 4; y++){
    //         std::cout << myTest[x][y] << " ";
    //     }
    //     std::cout << "\n";
    // }

    // std::cout << "glm matrix: \n";

    // for(int x = 0; x < 4; x++){
    //     for(int y = 0; y < 4; y++){
    //         std::cout << glmTest[x][y] << " ";
    //     }
    //     std::cout << "\n";
    // }

    // return 0;

    dem::ecs::RegisterComponent<dem::CameraComponent>();
    dem::ecs::RegisterComponent<dem::MeshRenderer>();
    dem::ecs::RegisterComponent<dem::TextRenderer>();
    dem::ecs::RegisterComponent<dem::LightSourceComponent>();
    
    dem::Renderer::Init(1280, 720);
    dem::Image texture;
    texture.Load("examples/texture.png");

    dem::Material mat{};
    mat.glImage.image = &texture;
    mat.Load();

    dem::ecs::Entity player = dem::ecs::CreateEntity();
    dem::TransformComponent* playerTransform = player.AddComponent<dem::TransformComponent>();
    playerTransform->rotation = dem::math::vec3(0.f, 0.f, 0.f);
    
    dem::ecs::Entity camera = dem::ecs::CreateEntity();
    dem::TransformComponent* cameraTransform = camera.AddComponent<dem::TransformComponent>();
    // cameraTransform->rotation[2] = PI;
    cameraTransform->parent = player.GetComponent<dem::TransformComponent>();
    dem::CameraComponent* cameraComponent = camera.AddComponent<dem::CameraComponent>();
    cameraComponent->fov = PI * 1.f / 2.f;

    dem::Mesh mesh;
    mesh.Load("examples\\cube.obj");

    dem::MeshRenderer* entityRenderer = entity.AddComponent<dem::MeshRenderer>();

    entityRenderer->material = &mat;
    entityRenderer->mesh = &mesh;
    entityRenderer->Load();

    dem::ecs::Entity lightEntity = dem::ecs::CreateEntity();
    dem::TransformComponent* lightTransform = lightEntity.AddComponent<dem::TransformComponent>();
    lightTransform->scale = dem::math::vec3(0.1f, 0.1f, 0.1f);
    dem::LightSourceComponent* light = lightEntity.AddComponent<dem::LightSourceComponent>();
    light->transform = lightTransform;
    dem::MeshRenderer* lightRenderer = lightEntity.AddComponent<dem::MeshRenderer>();
    lightRenderer->material = &mat;
    lightRenderer->mesh = &mesh;
    lightRenderer->Load();

    dem::Input::Init();
    dem::Input::sensivity = 3.f;

    float deltaTime = 0;
    int frameCounter = 0;

    float fpsDeltatime = 0;
    int fpsFrames = 160;
    while(!dem::Renderer::Render()){
        frameCounter++;
        
        
        if(dem::Input::GetKeyDown(dem::KeyCode::P)) cameraComponent->proj = (dem::Projection)!cameraComponent->proj; // !1 == 0 and !0 == 1

        if(dem::Input::GetAxisWheel() < 0.0f) cameraComponent->size /= 1.1f;
        if(dem::Input::GetAxisWheel() > 0.0f) cameraComponent->size *= 1.1f;

        cameraTransform->rotation += dem::math::vec3(2.5f, 0.f, 0.f) * dem::Input::GetAxisY() * deltaTime;
        playerTransform->rotation += dem::math::vec3(0.f, 2.5f, 0.f) * dem::Input::GetAxisX() * deltaTime;
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

        if(dem::Input::GetKey(dem::KeyCode::E)) playerTransform->position += dem::math::vec3(0.f, 0.5f * deltaTime, 0.f);
        if(dem::Input::GetKey(dem::KeyCode::Q)) playerTransform->position -= dem::math::vec3(0.f, 0.5f * deltaTime, 0.f);
        
        // lightTransform->position = dem::math::vec3(-3.0, 0.0, 0.0);
        lightTransform->position = dem::math::vec3(std::sin(glfwGetTime() / 5.0), 0.f, std::cos(glfwGetTime() / 5.0)) * 3.f;

        //std::cout << "                                                                                                                              \n";
        // std::cout << "                                                                                                                              \r";
        // std::cout << "cameraPos: " << playerTransform->position[0] << " " << playerTransform->position[1] << " " <<  playerTransform->position[2] << "\r";
        //std::cout << "cameraRot: " << cameraTransform->rotation[0] << " " << cameraTransform->rotation[1] << " " <<  cameraTransform->rotation[2] << "\r";
        //entityTransform->rotation = entityTransform->rotation + dem::math::vec3(0.f, 0.f, (float) deltaTime * 1.2f);

        fpsDeltatime += deltaTime;
        fpsFrames;
        if(frameCounter % fpsFrames == 0){
            // logger->log("fps: ", (int)(1. / (fpsDeltatime / (float)fpsFrames)));
            fpsDeltatime = 0;
        }
        
        dem::Input::Update();
        dem::Time::Update();
        deltaTime = dem::Time::GetDeltaTime();
    }

    logger->log("Total frames generated: ", frameCounter);


    return 0;
}