#include "core/Renderer/Render.h"

#include "core/ECS/ecs.h"
#include "core/Components/TransformComponent.h"
#include "core/Components/CameraComponent.h"
#include "core/Components/MeshRenderer.h"

namespace dem{

GLFWwindow* Renderer::window = NULL;

int counter;

Image* image;
Renderer::GLImage* glImage;

int Renderer::Init(int width, int height){
    Logger::get()->log("Initializing Renderer");

    Logger* logger = Logger::get();

    if(glfwInit()) logger->log("GLFW Initialized");
    else {
        logger->log("ERROR: glfw initialization failed");
        return -1;
    }

    window = glfwCreateWindow(width, height, "DEMEngine", NULL, NULL);
    if(!window) logger->log("ERROR: cannot create window");

    glfwMakeContextCurrent(window);
    gladLoadGL();

    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST); 

    return 0;
}

int Renderer::Render(){
    TransformComponent* cameraTransform;
    CameraComponent* camera;

    for(int i = 0; i < ecs::entities.size(); i++){
        camera = ecs::entities[i].GetComponent<CameraComponent>();
        if(camera != NULL){
            cameraTransform = ecs::entities[i].GetComponent<TransformComponent>();
            if(cameraTransform != NULL){
                break;
            }
        }
    }

    if(camera == NULL){
        Logger::get()->log("ERROR: cannot find entity with camera component to render");
        return glfwWindowShouldClose(window);
    }
    if(cameraTransform == NULL){
        Logger::get()->log("ERROR: cannot find transform component of the camera");
        return glfwWindowShouldClose(window);
    }

    math::mat4 projection = camera->GetProjectionMatrix();
    math::mat4 model_matrix;
    math::mat4 view_matrix = cameraTransform->GetModelMatrix();
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    TransformComponent* transform;
    MeshRenderer* meshRenderer;
    
    for(int i = 0; i < ecs::entities.size(); i++){
        meshRenderer = ecs::entities[i].GetComponent<MeshRenderer>();
        if(meshRenderer == NULL) continue;
        transform = ecs::entities[i].GetComponent<TransformComponent>();
        if(transform == NULL) continue;
        
        //Logger::get()->log("got entity with id: ", (unsigned)ecs::entities[i].id);

        glUniformMatrix4fv(meshRenderer->program->mat4_projection_location, 1, GL_FALSE, (GLfloat*)projection.data);
        glUniformMatrix4fv(meshRenderer->program->mat4_view_location, 1, GL_FALSE, (GLfloat*)view_matrix.data);

        glUseProgram(meshRenderer->program->gl);
        model_matrix = transform->GetModelMatrix();
        
        glUniformMatrix4fv(meshRenderer->program->mat4_model_location, 1, GL_FALSE, (GLfloat*)model_matrix.data);
        glBindVertexArray(meshRenderer->mesh->VAO.gl);
        glDrawArrays(GL_TRIANGLES, 0, meshRenderer->mesh->VAO.VBO.verticies_size / 3);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();

    return glfwWindowShouldClose(window);
}
}