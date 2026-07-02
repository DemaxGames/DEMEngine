#include "core/Renderer/Render.h"

#include "core/ECS/ecs.h"
#include "core/Components/TransformComponent.h"
#include "core/Components/CameraComponent.h"
#include "core/Components/MeshRenderer.h"
#include "core/Components/TextRenderer.h"
#include "core/Renderer/TextRendering.h"

namespace dem{

GLFWwindow* Renderer::window = NULL;

int counter;

Image* image;
Renderer::GLImage* glImage;

void APIENTRY openGLDebugCallback(GLenum source, GLenum type, GLuint id, 
                                  GLenum severity, GLsizei length, 
                                  const GLchar* message, const void* userParam) {
    // Ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

    std::cerr << "---------------" << std::endl;
    std::cerr << "Debug message (" << id << "): " << message << std::endl;

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:         std::cerr << "Severity: HIGH"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cerr << "Severity: MEDIUM"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cerr << "Severity: LOW"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cerr << "Severity: NOTIFICATION"; break;
    }
    std::cerr << std::endl;
}

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

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    glfwSwapInterval(0);
    glEnable(GL_DEPTH_TEST); 

    int flags; 
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT);
        // Forces the callback to execute on the same thread as the error-causing call
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
        glDebugMessageCallback(openGLDebugCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    TextInit();

    return 0;
}

void RenderText(TextRenderer* textRenderer){
    glUseProgram(Renderer::sharedProgram->gl);

    glUniform2fv(Renderer::offsets_location, 100, textRenderer->offsets->data);
    glUniform1iv(Renderer::data_location, 100, textRenderer->data);
    
    glActiveTexture(Renderer::sharedGLImage->slot);
    glBindTexture(GL_TEXTURE_2D, Renderer::sharedGLImage->gl);
    glUniform1i(Renderer::sharedTex_location, Renderer::sharedGLImage->slot - GL_TEXTURE0);

    glBindVertexArray(textRenderer->VAO.gl);
    glDepthFunc(GL_ALWAYS);
    glDrawArraysInstanced(GL_TRIANGLES, 0, Renderer::sharedVBO->data_size / 2, textRenderer->str.length());
    glDepthFunc(GL_LESS);
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
    glClearColor(0.1, 0.1, 0.1, 1.0);
    
    TransformComponent* transform;
    MeshRenderer* meshRenderer;
    TextRenderer* textRenderer;
    
    for(int i = 0; i < ecs::entities.size(); i++){
        meshRenderer = ecs::entities[i].GetComponent<MeshRenderer>();
        if(transform == NULL) continue;
        textRenderer = ecs::entities[i].GetComponent<TextRenderer>();
        if(textRenderer != NULL){
            RenderText(textRenderer);
            continue;
        }
        if(meshRenderer == NULL) continue;
        transform = ecs::entities[i].GetComponent<TransformComponent>();
        //Logger::get()->log("got entity with id: ", (unsigned)ecs::entities[i].id);
        glUseProgram(meshRenderer->material->program.gl);

        glActiveTexture(meshRenderer->material->glImage.slot);

        glUniformMatrix4fv(meshRenderer->material->mat4_projection_location, 1, GL_FALSE, (GLfloat*)projection.data);
        glUniformMatrix4fv(meshRenderer->material->mat4_view_location, 1, GL_FALSE, (GLfloat*)view_matrix.data);
        glBindTexture(GL_TEXTURE_2D, meshRenderer->material->glImage.gl);
        glUniform1i(meshRenderer->material->sampler2D_tex, meshRenderer->material->glImage.slot - GL_TEXTURE0);

        model_matrix = transform->GetModelMatrix();
        
        glUniformMatrix4fv(meshRenderer->material->mat4_model_location, 1, GL_FALSE, (GLfloat*)model_matrix.data);
        glBindVertexArray(meshRenderer->VAO.gl);
        glDrawArrays(GL_TRIANGLES, 0, meshRenderer->VAO.VBO.data_size / 3);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();

    return glfwWindowShouldClose(window);
}
}