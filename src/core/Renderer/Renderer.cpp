#include "core/Renderer/Render.h"

namespace dem{

GLFWwindow* Renderer::window = NULL;
dem::Renderer::GLProgram* Renderer::program = NULL;

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

int Renderer::LoadScene(Scene& scene){
    Logger::get()->log("Loading scene");

    Renderer::Shader* vertex_shader = new Renderer::Shader("examples/shader.vert", GL_VERTEX_SHADER);
    vertex_shader->Compile();
    Renderer::Shader* fragment_shader = new Renderer::Shader("examples/shader.frag", GL_FRAGMENT_SHADER);
    fragment_shader->Compile();
    program = new Renderer::GLProgram(vertex_shader, fragment_shader);
    program->Link();

    for(int i = 0; i < scene.objects.size(); i++){
        scene.objects[i]->mesh->VAO.program = program;
        scene.objects[i]->mesh->VAO.VBO = Renderer::VertexBuffer(scene.objects[i]->mesh->verticies, scene.objects[i]->mesh->verticies_count * 3);
        if(scene.objects[i]->mesh->drawElementsSupport) 
            scene.objects[i]->mesh->VAO.EBO = Renderer::ElementBuffer(scene.objects[i]->mesh->indicies, scene.objects[i]->mesh->indicies_count * 3);
        scene.objects[i]->mesh->VAO.BindAll();
    }

    return 0;
}

int Renderer::Render(Scene& scene){
    math::mat4 projection = scene.camera->GetProjectionMatrix();
    math::mat4 model_matrix;
    math::mat4 view_matrix = scene.camera->GetModelMatrix();
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniformMatrix4fv(program->mat4_projection_location, 1, GL_FALSE, (GLfloat*)projection.data);
    glUniformMatrix4fv(program->mat4_view_location, 1, GL_FALSE, (GLfloat*)view_matrix.data);

    for(int i = 0; i < scene.objects.size(); i++){
        glUseProgram(program->gl);
        
        model_matrix = scene.objects[i]->GetModelMatrix();
        
        glUniformMatrix4fv(program->mat4_model_location, 1, GL_FALSE, (GLfloat*)model_matrix.data);
        glBindVertexArray(scene.objects[i]->mesh->VAO.gl);
        if(scene.objects[i]->mesh->drawElementsSupport){
            glDrawElements(GL_TRIANGLES, scene.objects[i]->mesh->VAO.EBO.size, GL_UNSIGNED_INT, NULL);
        }
        else {
            glDrawArrays(GL_TRIANGLES, 0, scene.objects[i]->mesh->VAO.VBO.size / 3);
        }
    }

    glfwSwapBuffers(window);
    glfwPollEvents();

    return glfwWindowShouldClose(window);
}
}