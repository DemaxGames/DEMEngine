#include "core/Components/TextRenderer.h"
#include "core/Renderer/TextRendering.h"
#include "core/Logger/Logger.h"

namespace dem{

math::vec3 FontPixelLocationToUV(int x, int y, int width, Image* pImage){
    math::vec3 result;
    result[2] = (float)width / (float)(pImage->width);
    result[0] = (float)x / (float)(pImage->width);
    result[1] = (float)width / (float)(pImage->height);
    return result;
}

math::vec3 charToUV(char symbol){
    switch(symbol){
        case 'A':
            return FontPixelLocationToUV(0, 0, 10 + 3, Renderer::sharedGLImage->image);
            break;
        case 'B':
            return FontPixelLocationToUV(19, 0, 10 + 3, Renderer::sharedGLImage->image);
            break;
        case 'C':
            return FontPixelLocationToUV(33, 0, 40-33+1, Renderer::sharedGLImage->image);
            break;
        case 'D':
            return FontPixelLocationToUV(49, 0, 60-49+1, Renderer::sharedGLImage->image);
            break;
        case 'E':
            return FontPixelLocationToUV(70, 0, 80-70+1, Renderer::sharedGLImage->image);
            break;
        case 'F':
            return FontPixelLocationToUV(87, 0, 99-87 + 1, Renderer::sharedGLImage->image);
            break;
        default:
            return FontPixelLocationToUV(0, 0, 10 + 1, Renderer::sharedGLImage->image);
            break;
    }
}

void TextRenderer::Load(){
    if(transform == nullptr) {
        Logger::get()->log("ERROR: cannot load text renderer component, the transform is nullptr");
        return;
    }
    math::vec2 curOffset(transform->position[0], transform->position[1]);
    for(int i = 0; i < str.length() && i < 100; i++){
        textureUV[i] = charToUV(str[i]);
        offsets[i] = curOffset;
        curOffset[0] += textureUV[i][2];
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