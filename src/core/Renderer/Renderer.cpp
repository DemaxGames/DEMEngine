#include "core/Renderer/Render.h"

namespace dem{

GLFWwindow* Renderer::window = NULL;
dem::Renderer::GLProgram* Renderer::program = NULL;
dem::Renderer::VertexArray* Renderer::vertexArray = NULL;
std::vector<dem::Renderer::VertexBuffer> Renderer::vertexBuffers = std::vector<dem::Renderer::VertexBuffer>();

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

    return 0;
}

int Renderer::LoadScene(Scene scene){
    Logger::get()->log("Loading scene");

    vertexBuffers.clear();

    for(Object object : scene.objects){
        vertexBuffers.push_back(Renderer::VertexBuffer(object.mesh.verticies.data(), object.mesh.verticies.size()));
    }

    Renderer::Shader* vertex_shader = new Renderer::Shader("shader.vert", GL_VERTEX_SHADER);
    vertex_shader->Compile();
    Renderer::Shader* fragment_shader = new Renderer::Shader("shader.frag", GL_FRAGMENT_SHADER);
    fragment_shader->Compile();
    program = new Renderer::GLProgram(vertex_shader, fragment_shader);
    program->Link();

    vertexArray = new Renderer::VertexArray();

    return 0;
}

int Renderer::Render(Scene scene){
    math::mat4 projection = scene.camera->GetProjectionMatrix();
    math::mat4 model_matrix;
    math::mat4 view_matrix = scene.camera->GetModelMatrix();

    glClear(GL_COLOR_BUFFER_BIT);

    for(int i = 0; i < vertexBuffers.size(); i++){
        vertexArray->CreateVertexAttribPointer(*program);
        glUseProgram(program->gl);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[i].gl);
        glUniformMatrix4fv(program->mat4_projection_location, 1, GL_FALSE, (GLfloat*)projection.data);
        glUniformMatrix4fv(program->mat4_view_location, 1, GL_FALSE, (GLfloat*)view_matrix.data);
        model_matrix = scene.objects[i].GetModelMatrix();
        glUniformMatrix4fv(program->mat4_model_location, 1, GL_FALSE, (GLfloat*)model_matrix.data);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();

    return glfwWindowShouldClose(window);
}
}