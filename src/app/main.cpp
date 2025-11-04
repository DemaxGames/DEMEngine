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
#include <stdio.h>
#include <filesystem>
#include <vector>


int main(){
    dem::Logger* logger = dem::Logger::get();
    
    dem::Mesh* mesh = new dem::Mesh();
    mesh->Load("examples/cube.obj");
    
    dem::Object* object = new dem::Object(mesh);
    object->position = dem::math::vec3(0.5f, 0.0f, -15.4f);
    
    dem::Mesh* doritosMesh = new dem::Mesh();
    doritosMesh->Load("examples/doritos.obj");

    dem::Object* doritos = new dem::Object(doritosMesh);
    doritos->position = dem::math::vec3(-2.5f, 0.0f, -0.4f);
    
    dem::Camera* cam = new dem::Camera();
    cam->nearClip = 0.005f;
    cam->fov = 3.14/1.7f;
    
    dem::Scene scene;
    scene.camera = cam;
    scene.objects.push_back(object);
    scene.objects.push_back(doritos);
    
    dem::Renderer::Init(1920, 1080);
    dem::Renderer::LoadScene(scene);

    dem::Input::Init();
    dem::Input::sensivity = 3.f;

    float deltaTime = 0;
    int frameCounter = 0;
    while(!dem::Renderer::Render(scene)){
        frameCounter++;

        cam->rotation += dem::math::vec3(0.f, 1.5f, 0.f) * dem::Input::GetAxisX() * deltaTime;
        cam->localRotation += dem::math::vec3(1.5f, 0.f, 0.f) * dem::Input::GetAxisY() * deltaTime;

        if(dem::Input::GetKey(dem::KeyCode::W)) cam->position += cam->forward * 1.5f * deltaTime;
        if(dem::Input::GetKey(dem::KeyCode::S)) cam->position += cam->forward * -1.5f * deltaTime;
        if(dem::Input::GetKey(dem::KeyCode::A)) cam->position += cam->right * -1.5f * deltaTime;
        if(dem::Input::GetKey(dem::KeyCode::D)) cam->position += cam->right * 1.5f * deltaTime;

        object->rotation = object->rotation + dem::math::vec3((float) deltaTime * 0.09f, 0.f, 0.f);

        dem::Input::Update();
        dem::Time::Update();
        deltaTime = dem::Time::GetDeltaTime();
        //std::cout << "\rfps: " << 1.f / deltaTime;
    }

    logger->log("Total frames generated: ", frameCounter);

    return 0;
}