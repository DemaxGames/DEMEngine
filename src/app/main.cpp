#include <iostream>
#include <stdio.h>
#include <filesystem>
#include <vector>
#include <windows.h>
#include "core/dem.h"
#include "core/time/time.h"
#include <cmath>
#include <vector>

#define PI 3.141592f

int shroomCount = 0;

std::vector<dem::ecs::Entity> shrooms;
std::vector<dem::TransformComponent*> shroomsTransform;
std::vector<dem::MeshRenderer*> shroomsRenderer;

void AddShroom(dem::Mesh *shroomMesh, dem::Material *shroomMaterial){
    int i = shroomCount;
    shrooms.push_back(dem::ecs::CreateEntity());
    shroomsTransform.push_back(shrooms[i].AddComponent<dem::TransformComponent>());
    shroomsRenderer.push_back(shrooms[i].AddComponent<dem::MeshRenderer>());
    shroomsTransform[i]->position = dem::math::vec3((float)i / 10.f, 0.0, 0.0);
    shroomsTransform[i]->scale = dem::math::vec3(0.01, 0.01, 0.01);
    shroomsRenderer[i]->mesh = shroomMesh;
    shroomsRenderer[i]->material = shroomMaterial;
    shroomsRenderer[i]->Load();
    shroomCount++;
}

int main(){
    dem::Logger *logger = dem::Logger::get();

    dem::math::mat4 testa(1.f, 2.f, 3.f, 2.f,
                     -2.f, 1.f, 7.f, -2.f,
                     3.f, 4.f, 1.f, 4.f,
                     2.f, 3.f, 1.f, 1.f);

    dem::math::vec4 testb(-5.f, 2.f, 2.f, 2.f);

    dem::math::vec4 result = testa * testb;



    dem::Logger::get()->log("testa: ", testa);
    dem::Logger::get()->log("testb: ", testb);
    dem::Logger::get()->log("result: ", result);

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
    
    dem::CharSet font;
    font.Load("examples/font.txt");

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
    cameraTransform->position = dem::math::vec3(0.0, 0.0, 0.0);
    // cameraTransform->rotation[2] = PI;
    cameraTransform->parent = player.GetComponent<dem::TransformComponent>();
    dem::CameraComponent* cameraComponent = camera.AddComponent<dem::CameraComponent>();
    cameraComponent->pTransform = cameraTransform;
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

    dem::ecs::Entity textEntity = dem::ecs::CreateEntity();
    dem::TransformComponent* textTransform = textEntity.AddComponent<dem::TransformComponent>();
    dem::TextRenderer* textRenderer = textEntity.AddComponent<dem::TextRenderer>();
    textRenderer->color = dem::math::vec3(0.0, 0.5, 0.7);
    textRenderer->transform = textTransform;
    textRenderer->pCharSet = &font;
    textRenderer->Load();
    textRenderer->SetText("QUICK BROWN FOX JUMPS OVER THE LAZY DOG");
    
    textTransform->position = dem::math::vec3(-0.4, 0, 0.0);

    dem::Mesh shroomMesh;
    shroomMesh.name = "Shroom";
    shroomMesh.Load("examples/butteryFaggot.obj");

    dem::Image shroomTexture;
    shroomTexture.Load("examples/butteryFaggot.png");

    dem::Material shroomMaterial{};
    shroomMaterial.glImage.image = &shroomTexture;
    shroomMaterial.Load();

    for(int i = 0; i < 10; i++){
        AddShroom(&shroomMesh, &shroomMaterial);
    }

    float deltaTime = 0;
    int frameCounter = 0;

    float fpsUpdateDeltatime = 0.5;
    float fpsDeltatime = 0;
    while(!dem::Renderer::Render()){
        frameCounter++;
        
        if(dem::Input::GetKeyDown(dem::KeyCode::M)) AddShroom(&shroomMesh, &shroomMaterial);
        
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

        // logger->log("PLAYERPOS: ", playerTransform->position);

        if(dem::Input::GetKey(dem::KeyCode::E)) playerTransform->position += dem::math::vec3(0.f, 0.5f * deltaTime, 0.f);
        if(dem::Input::GetKey(dem::KeyCode::Q)) playerTransform->position -= dem::math::vec3(0.f, 0.5f * deltaTime, 0.f);

        if(dem::Input::GetKey(dem::KeyCode::R)) entityTransform->position += dem::math::vec3(0.f, 0.5f * deltaTime, 0.f);
        if(dem::Input::GetKey(dem::KeyCode::F)) entityTransform->position -= dem::math::vec3(0.f, 0.5f * deltaTime, 0.f);
        

        lightTransform->position = dem::math::vec3(-3.0, 1.0, 0.0);
        // lightTransform->position = dem::math::vec3(std::sin(glfwGetTime() / 5.0), 0.f, std::cos(glfwGetTime() / 5.0)) * 3.f;

        //std::cout << "                                                                                                                              \n";
        // std::cout << "                                                                                                                              \r";
        // std::cout << "cameraPos: " << playerTransform->position[0] << " " << playerTransform->position[1] << " " <<  playerTransform->position[2] << "\r";
        //std::cout << "cameraRot: " << cameraTransform->rotation[0] << " " << cameraTransform->rotation[1] << " " <<  cameraTransform->rotation[2] << "\r";
        //entityTransform->rotation = entityTransform->rotation + dem::math::vec3(0.f, 0.f, (float) deltaTime * 1.2f);

        fpsDeltatime += deltaTime;
        if(fpsDeltatime > fpsUpdateDeltatime){
            // logger->log("fps: ", (int)(1. / (fpsDeltatime / (float)fpsFrames)));
            textRenderer->SetText("FPS: " + std::to_string((int)(1. / (fpsDeltatime / frameCounter))) );
            frameCounter = 0;
            fpsDeltatime = 0;
        }


        
        dem::Input::Update();
        dem::Time::Update();
        deltaTime = dem::Time::GetDeltaTime();
    }

    logger->log("Total frames generated: ", frameCounter);


    return 0;
}