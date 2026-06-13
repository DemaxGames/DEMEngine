#include "core/Renderer/VertexArray.h"

namespace dem{

Renderer::VertexArray::VertexArray(){
    Logger::get()->log("Creating Vertex Array");

    material = NULL;
    EBO.size = 0;
}

int Renderer::VertexArray::BindAll(){
    Logger::get()->log("Binding to vertexArray");

    size_t stride = 0;
    glBindBuffer(GL_ARRAY_BUFFER, VBO.gl);
    glGenVertexArrays(1, &gl);
    glBindVertexArray(gl);
    if(material != NULL){
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO.gl);
        glEnableVertexAttribArray(material->vec3_vpos_location);
        glVertexAttribPointer(material->vec3_vpos_location, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
        glEnableVertexAttribArray(material->vec3_normal_location);
        glVertexAttribPointer(material->vec3_normal_location, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(material->vec2_uv_location);
        glVertexAttribPointer(material->vec2_uv_location, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    } else{
        Logger::get()->log("ERROR: cannot BindAll because material pointer is not set");
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