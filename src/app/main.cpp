#include <iostream>
#include "core/Logger/Logger.h"
#include "core/Engine/Mesh.h"
#include "core/Engine/Object.h"
#include "core/Engine/Scene.h"
#include "core/Renderer/Render.h"
#include "core/math/vec.h"
#include "core/math/mat.h"
#include <stdio.h>
#include <filesystem>
#include <vector>

std::vector<dem::math::vec3> verticies[] = {{
    dem::math::vec3(0.5f, 0.5f, -1.5f),
    dem::math::vec3(0.6f, 0.5f, -1.f),
    dem::math::vec3(0.55f, 0.4f, -1.f),},
    {dem::math::vec3(-0.5f, 0.5f, -1.f),
    dem::math::vec3(-0.6f, 0.5f, -1.5f),
    dem::math::vec3(-0.55f, 0.4f, -1.f),},
    {dem::math::vec3(-0.5f, -0.5f, -1.f),
    dem::math::vec3(-0.6f, -0.5f, -1.f),
    dem::math::vec3(-0.55f, -0.4f, -1.5f)}
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

    dem::Camera* cam = new dem::Camera();
    float fov = 4.f;
    cam->projection.projection(1280.f/720.f, 3.14f/fov, 0.1f, 200.f);

    dem::Scene scene;
    scene.objects.push_back(objects[0]);
    scene.objects.push_back(objects[1]);
    scene.objects.push_back(objects[2]);

    scene.camera = cam;

    dem::Renderer::Init(1280, 720);
    dem::Renderer::LoadScene(scene);

    int frameCounter = 0;
    while(!dem::Renderer::Render(scene)){
        cam->projection.projection(1280.f/720.f, 3.14f/fov, 0.1f, 200.f);
        fov-=0.01f;
        frameCounter++;
    }

    logger->log("Total frames generated: ", frameCounter);

    return 0;
}