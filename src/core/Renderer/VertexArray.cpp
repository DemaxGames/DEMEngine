#include "core/Renderer/VertexArray.h"

dem::Renderer::VertexArray::VertexArray(){
    Logger::get()->log("Creating Vertex Array");

    glGenVertexArrays(1, &gl);
}

int dem::Renderer::VertexArray::CreateVertexAttribPointer(GLProgram program){
    GLint vpos_location = glGetAttribLocation(program.gl, "vPos");

    glBindVertexArray(gl);
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

    return 0;
}