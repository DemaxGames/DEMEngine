#include "core/Renderer/ElementBuffer.h"

namespace dem{

Renderer::ElementBuffer::ElementBuffer(){

}

Renderer::ElementBuffer::ElementBuffer(unsigned int* indicies, size_t size){
    Logger::get()->log("Creating Element Buffer");

    if(indicies != NULL) this->indicies = indicies;
    else{
        Logger::get()->log("ERROR: Cannot create Element Buffer, indicies is NULL");
        return;
    }
    if(size != 0) this->size = size;
    else{
        Logger::get()->log("ERROR: Cannot create Element Buffer, size is 0");
        return;
    }
    
    glGenBuffers(1, &gl);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * this->size, this->indicies, GL_STATIC_DRAW);
}
}