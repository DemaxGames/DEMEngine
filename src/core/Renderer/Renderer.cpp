#include "core/Renderer/Render.h"



int dem::Renderer::Init(int width, int height){
    Logger::get()->log("Initializing Renderer");

    dem::Logger* logger = dem::Logger::get();

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

int dem::Renderer::LoadScene(dem::Scene scene){
    Logger::get()->log("Loading scene");

    vertexBuffers.clear();

    for(dem::Object object : scene.objects){
        vertexBuffers.push_back(dem::Renderer::VertexBuffer(object.mesh.verticies.data(), object.mesh.verticies.size()));
    }

    dem::Renderer::Shader* vertex_shader = new dem::Renderer::Shader("shader.vert", GL_VERTEX_SHADER);
    vertex_shader->Compile();
    dem::Renderer::Shader* fragment_shader = new dem::Renderer::Shader("shader.frag", GL_FRAGMENT_SHADER);
    fragment_shader->Compile();
    program = new dem::Renderer::GLProgram(vertex_shader, fragment_shader);
    program->Link();

    vertexArray = new dem::Renderer::VertexArray();

    return 0;
}

int dem::Renderer::Render(){

    glClear(GL_COLOR_BUFFER_BIT);
    
    for(int i = 0; i < vertexBuffers.size(); i++){
        vertexArray->CreateVertexAttribPointer(*program);
        glUseProgram(program->gl);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[i].gl);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();

    return glfwWindowShouldClose(window);
}