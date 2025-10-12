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

std::vector<dem::math::vec3> verticies {
    dem::math::vec3(0.f, 0.05f, 0.f),
    dem::math::vec3(-0.053f, -0.01, 0.f),
    dem::math::vec3(0.045, -0.03, 0.f)
};

int main(){
    dem::Logger* logger = dem::Logger::get();

    dem::Mesh meshes[3];
    meshes[0].verticies = verticies;
    meshes[1].verticies = verticies;
    meshes[2].verticies = verticies;

    dem::Object objects[3];
    objects[0].mesh = meshes[0];
    objects[1].mesh = meshes[1];
    objects[2].mesh = meshes[2];

    objects[0].position = dem::math::vec3(0.5f, 0.2f, -0.4f);
    objects[1].position = dem::math::vec3(-0.5f, -0.25f, -0.5f);
    objects[2].position = dem::math::vec3(0.0f, 0.0f, -0.4f);
    

    dem::Camera* cam = new dem::Camera();
    cam->nearClip = 0.005f;
    cam->fov = 3.14/1.7f;

    dem::Scene scene;
    scene.objects.push_back(objects[0]);
    scene.objects.push_back(objects[1]);
    scene.objects.push_back(objects[2]);

    scene.camera = cam;
    
    dem::Renderer::Init(1280, 720);
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

        scene.objects[0].rotation = dem::math::vec3((float)frameCounter / 100.f, 0.f, 0.f);
        scene.objects[1].rotation = dem::math::vec3(0.0f, (float)frameCounter / 100.f, 0.f);
        scene.objects[2].rotation = dem::math::vec3(0.f, 0.f, (float)frameCounter / 100.f);

        dem::Input::Update();
        dem::Time::Update();
        deltaTime = dem::Time::GetDeltaTime();
        std::cout << "\rfps: " << 1.f / deltaTime;
    }

    logger->log("Total frames generated: ", frameCounter);

    return 0;
}


