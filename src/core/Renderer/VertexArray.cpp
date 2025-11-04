#include "core/Renderer/VertexArray.h"

namespace dem{

Renderer::VertexArray::VertexArray(){
    Logger::get()->log("Creating Vertex Array");

    program = NULL;
    VBO.verticies_size = 0;
    VBO.normals_size = 0;
    EBO.size = 0;
}

int Renderer::VertexArray::BindAll(){
    Logger::get()->log("Binding to vertexArray");

    size_t stride = 0;
    glGenVertexArrays(1, &gl);
    glBindVertexArray(gl);
    if(program != NULL){
        if(VBO.verticies_size > 0){
            stride += 3 * sizeof(float);
        }
        if(VBO.normals_size > 0){
            stride += 3 * sizeof(float);
        }
        if(EBO.size > 0){
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO.gl);
        }
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO.gl);
        glEnableVertexAttribArray(program->vec3_vpos_location);
        glVertexAttribPointer(program->vec3_vpos_location, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
        if(VBO.normals_size > 0){
            glEnableVertexAttribArray(program->vec3_normal_location);
            glVertexAttribPointer(program->vec3_normal_location, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        }
    } else{
        Logger::get()->log("ERROR: cannot BindAll because program pointer is not set");
        return -1;
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