#include "core/Components/TextRenderer.h"
#include "core/Renderer/TextRendering.h"
#include "core/Logger/Logger.h"

namespace dem{

void TextRenderer::Load(){
    if(transform == nullptr) {
        Logger::get()->log("ERROR: cannot load text renderer component, the transform is nullptr");
        return;
    }
    math::vec2 curOffset(transform->position[0], transform->position[1]);
    for(int i = 0; i < str.length() && i < 100; i++){
        offsets[i] = curOffset;
        curOffset += math::vec2(0.2, 0.0);
    }   

    for(int i = 0; i < str.length() && i < 100; i++){
        data[i] = str.data()[i];
    }

    glGenVertexArrays(1, &VAO.gl);
    glBindVertexArray(VAO.gl);
    glBindBuffer(GL_ARRAY_BUFFER, Renderer::sharedVBO->gl);
    GLint vPos_location = glGetAttribLocation(Renderer::sharedProgram->gl, "vPos");
    glEnableVertexAttribArray(vPos_location);
    glVertexAttribPointer(vPos_location, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glBindVertexArray(0);
}

}