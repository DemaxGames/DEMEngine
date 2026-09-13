#include "core/Components/TextRenderer.h"
#include "core/Renderer/TextRendering.h"
#include "core/Logger/Logger.h"

namespace dem{

void CharSet::Load(std::string path){
    std::ifstream file(path);
    std::string line;

    int l = 1;
    while(std::getline(file, line)){
        int i = 0;
        size_t length = line.length();

        std::string symbol, x, y, width;

        for(; i < length & line[i] != ' '; i++){
            symbol += line[i];
        }
        i++;
        for(; i < length & line[i] != ' '; i++){
            x += line[i];
        }
        i++;
        for(; i < length & line[i] != ' '; i++){
            y += line[i];
        }
        i++;
        for(; i < length; i++){
            width += line[i];
        }

        int index = symbol[0];
        if(index < 0 | index > 255){
            Logger::get()->log("ERROR: Cannot load charset " + path);
            Logger::get()->log("\tThe problem is in the beginning of the line ", l);

        }

        charInfos[index] = (CharInfo){index, std::stoi(x), std::stoi(y), std::stoi(width)};

        l++;
    }

    for(int i = 0; i < 256; i++){
        // std::cout << "for symbol '" << charInfos[i].symbol << "' x= " << charInfos[i].x << " y= " << charInfos[i].y << " width=" << charInfos[i].width << "\n";
    }
}


TextRenderer::TextRenderer(){
    color = math::vec3(1.0, 1.0, 1.0);
    transform = nullptr;
    str = "";
    changed = false;
}

math::vec3 FontPixelLocationToUV(int x, int y, int width, Image* pImage){
    math::vec3 result;
    result[2] = (float)width / (float)(pImage->width);
    result[0] = (float)x / (float)(pImage->width);
    result[1] = (float)width / (float)(pImage->height);
    return result;
}

math::vec3 charToUV(char symbol, CharSet *pCharSet){
    int x, y, width;
    x = pCharSet->charInfos[symbol].x;
    y = pCharSet->charInfos[symbol].y;
    width = pCharSet->charInfos[symbol].width;

    return FontPixelLocationToUV(x, y, width, Renderer::sharedGLImage->image);
}

void TextRenderer::LoadOffsets(){
    if(transform == nullptr) {
        Logger::get()->log("ERROR: cannot load text renderer component, the transform is nullptr");
        return;
    }
    math::vec2 curOffset(transform->position[0], transform->position[1]);
    for(int i = 0; i < str.length() && i < 100; i++){
        if(str[i] == ' '){
            textureUV[i] = math::vec3(0, 0, 0.002);
            offsets[i] = curOffset;
            curOffset[0] += textureUV[i][2];
        }

        textureUV[i] = charToUV(str[i], pCharSet);
        offsets[i] = curOffset;
        curOffset[0] += textureUV[i][2] + 0.002f;
    }
    if(changed) changed = false;
}

void TextRenderer::Load(){
    glGenVertexArrays(1, &VAO.gl);
    glBindVertexArray(VAO.gl);
    glBindBuffer(GL_ARRAY_BUFFER, Renderer::sharedVBO->gl);
    GLint vPos_location = glGetAttribLocation(Renderer::sharedProgram->gl, "vPos");
    glEnableVertexAttribArray(vPos_location);
    glVertexAttribPointer(vPos_location, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glBindVertexArray(0);
}

void TextRenderer::SetText(std::string text){
    str = text;
    changed = true;
}

bool TextRenderer::HasChanged(){
    return changed;
}

size_t TextRenderer::GetLength(){
    return str.length();
}


}