#include "core/Renderer/VertexArray.h"

dem::Renderer::VertexArray::VertexArray(){
    Logger::get()->log("Creating Vertex Array");

    glGenVertexArrays(1, &gl);
}

int dem::Renderer::VertexArray::CreateVertexAttribPointer(GLProgram program){
    glBindVertexArray(gl);
    glEnableVertexAttribArray(program.vec3_vpos_location);
    glVertexAttribPointer(program.vec3_vpos_location, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

    return 0;
}