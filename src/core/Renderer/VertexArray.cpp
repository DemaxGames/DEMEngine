#include "core/Renderer/VertexArray.h"

namespace dem{

Renderer::VertexArray::VertexArray(){
    Logger::get()->log("Creating Vertex Array");

    program = NULL;
}

int Renderer::VertexArray::BindAll(){
    glGenVertexArrays(1, &gl);
    glBindVertexArray(gl);
    if(program != NULL){
        glEnableVertexAttribArray(program->vec3_vpos_location);
        glVertexAttribPointer(program->vec3_vpos_location, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
    } else{
        Logger::get()->log("ERROR: cannot BindAll because program pointer is not set");
        return -1;
    }
    if(VBO.size > 0){
        glBindBuffer(GL_ARRAY_BUFFER, VBO.gl);
    }
    if(EBO.size > 0){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO.gl);
    }
    glBindVertexArray(0);

    return 0;
}

// int Renderer::VertexArray::CreateVertexAttribPointer(GLProgram program){
//     glBindVertexArray(gl);
//     glEnableVertexAttribArray(program.vec3_vpos_location);
//     glVertexAttribPointer(program.vec3_vpos_location, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

//     return 0;
// }
}