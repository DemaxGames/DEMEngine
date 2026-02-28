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
    logger->log("program");
    
    dem::Renderer::Shader* vertex_shader = new dem::Renderer::Shader("examples/shader.vert", GL_VERTEX_SHADER);
    vertex_shader->Compile();
    dem::Renderer::Shader* fragment_shader = new dem::Renderer::Shader("examples/shader.frag", GL_FRAGMENT_SHADER);
    fragment_shader->Compile();
    dem::Renderer::GLProgram* program = new dem::Renderer::GLProgram(vertex_shader, fragment_shader);
    program->Link();

    dem::ecs::Entity player = dem::ecs::CreateEntity();
    player.AddComponent<dem::TransformComponent>();
    dem::TransformComponent* playerTransform = player.GetComponent<dem::TransformComponent>();
    playerTransform->rotation = dem::math::vec3(0.f, 0.f, 0.f);
    
    dem::ecs::Entity camera = dem::ecs::CreateEntity();
    logger->log("camera id:", (uint64_t)camera.id);
    camera.AddComponent<dem::TransformComponent>();
    camera.AddComponent<dem::CameraComponent>();
    dem::TransformComponent* cameraTransform = camera.GetComponent<dem::TransformComponent>();
    cameraTransform->parent = player.GetComponent<dem::TransformComponent>();
    
    dem::CameraComponent* cameraComponent = camera.GetComponent<dem::CameraComponent>();
    cameraComponent->fov = PI * 1.f / 2.f;

    dem::Mesh mesh;
    mesh.Load("examples\\cube.obj");
    dem::Mesh mercedesMesh;
    mercedesMesh.Load("examples\\mercedes.obj");
    dem::ecs::Entity entity[20];
    for(int i = 0; i < 2; i++){
        entity[i] = dem::ecs::CreateEntity();
        entity[i].AddComponent<dem::TransformComponent>();
        entity[i].AddComponent<dem::MeshRenderer>();
        dem::TransformComponent* entityTransform = entity[i].GetComponent<dem::TransformComponent>();
        dem::MeshRenderer* meshRenderer = entity[i].GetComponent<dem::MeshRenderer>();
        *meshRenderer = (dem::MeshRenderer){
        .program = program,
        .mesh = &mesh};
        meshRenderer->Load();
    }
    dem::TransformComponent* entityTransform = entity[0].GetComponent<dem::TransformComponent>();
    entityTransform->position[0] += 1.5f;

    dem::ecs::Entity mercedes = dem::ecs::CreateEntity();
    mercedes.AddComponent<dem::TransformComponent>();
    mercedes.AddComponent<dem::MeshRenderer>();
    dem::TransformComponent* mercedesTransform = mercedes.GetComponent<dem::TransformComponent>();
    dem::MeshRenderer* mercedesMeshRenderer = mercedes.GetComponent<dem::MeshRenderer>();
    mercedesTransform->position[1] += 3.f;
    mercedesTransform->scale = dem::math::vec3(1./7., 1./7., 1./7.);
    mercedesMeshRenderer->program = program;
    mercedesMeshRenderer->mesh = &mercedesMesh;
    mercedesMeshRenderer->Load();

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

        std::cout << "                                                                                                                              \r";
        std::cout << "entitypos: " << entityTransform->position[0] << " " << entityTransform->position[1] << " " <<  entityTransform->position[2] << '\r';
        entityTransform->rotation = entityTransform->rotation + dem::math::vec3(0.f, 0.f, (float) deltaTime * 1.2f);

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