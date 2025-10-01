#include "core/Renderer/VertexBuffer.h"

dem::Renderer::VertexBuffer::VertexBuffer(float* verticies, size_t size){
    Logger::get()->log("Creating Vertex Buffer");

    if(verticies != NULL) this->verticies = verticies;
    else{
        dem::Logger::get()->log("ERROR: Cannot create Vertex Buffer, verticies is NULL");
        return;
    }
    if(size != 0) this->size = size;
    else{
        dem::Logger::get()->log("ERROR: Cannot create Vertex Buffer, size is 0");
        return;
    }
    
    glGenBuffers(1, &gl);
    glBindBuffer(GL_ARRAY_BUFFER, gl);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->size, this->verticies, GL_STATIC_DRAW);
}