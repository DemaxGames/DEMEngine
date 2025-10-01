#include <iostream>
#include "core/Logger/Logger.h"
#include "core/Engine/Mesh.h"
#include "core/Engine/Object.h"
#include "core/Engine/Scene.h"
#include "core/Renderer/Render.h"
#include <stdio.h>
#include <filesystem>
#include <vector>

std::vector<float> verticies[] = {{
    0.5f, 0.5f, 0.f,
    0.6f, 0.5f, 0.f,
    0.55f, 0.4f, 0.f},
    { -0.5f, 0.5f, 0.f,
    -0.6f, 0.5f, 0.f,
    -0.55f, 0.4f, 0.f},
    { -0.5f, -0.5f, 0.f,
    -0.6f, -0.5f, 0.f,
    -0.55f, -0.4f, 0.f}
};

int main(){
    dem::Logger* logger = dem::Logger::get();
    
    dem::Mesh meshes[3];
    meshes[0].verticies = verticies[0];
    meshes[1].verticies = verticies[1];
    meshes[2].verticies = verticies[2];

    dem::Object objects[3];
    objects[0].mesh = meshes[0];
    objects[1].mesh = meshes[1];
    objects[2].mesh = meshes[2];

    dem::Scene scene;
    scene.objects.push_back(objects[0]);
    scene.objects.push_back(objects[1]);
    scene.objects.push_back(objects[2]);

    dem::Renderer::Init(1280, 720);
    dem::Renderer::LoadScene(scene);

    uint64_t frameCounter = 0;
    while(!dem::Renderer::Render()){
        frameCounter++;
    }

    logger->log("Total frames generated: ");
    logger->log(std::to_string(frameCounter).data());

    return 0;
}