#include "core/Renderer/VertexBuffer.h"

namespace dem{

Renderer::VertexBuffer::VertexBuffer(){

}

Renderer::VertexBuffer::VertexBuffer(float* verticies, size_t size){
    Logger::get()->log("Creating Vertex Buffer");

    if(verticies != NULL) this->verticies = verticies;
    else{
        Logger::get()->log("ERROR: Cannot create Vertex Buffer, verticies is NULL");
        return;
    }
    if(size != 0) this->size = size;
    else{
        Logger::get()->log("ERROR: Cannot create Vertex Buffer, size is 0");
        return;
    }
    
    glGenBuffers(1, &gl);
    glBindBuffer(GL_ARRAY_BUFFER, gl);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->size, this->verticies, GL_STATIC_DRAW);
}

Renderer::VertexBuffer::VertexBuffer(math::vec3* verticies, size_t size){
    Logger::get()->log("Creating Vertex Buffer");

    if(verticies != NULL) this->verticies = (float*)verticies;
    else{
        Logger::get()->log("ERROR: Cannot create Vertex Buffer, verticies is NULL");
        return;
    }
    if(size != 0) this->size = size * 3;
    else{
        Logger::get()->log("ERROR: Cannot create Vertex Buffer, size is 0");
        return;
    }
    
    glGenBuffers(1, &gl);
    glBindBuffer(GL_ARRAY_BUFFER, gl);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->size * 3, this->verticies, GL_STATIC_DRAW);
}
}