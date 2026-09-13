#pragma once

#include <string>
#include "core/Renderer/VertexArray.h"
#include "core/Components/TransformComponent.h"

namespace dem{

    
struct CharInfo{
    unsigned char symbol;
    int x, y;
    int width;
};

class CharSet{
public:
    CharInfo charInfos[256] = {0};
    dem::Image *pImage;
    
    void Load(std::string path);
};

class TextRenderer{
public:
    math::vec3 color;
    CharSet *pCharSet;
    TransformComponent* transform;
    Renderer::VertexArray VAO;
    math::vec2 offsets[100];
    math::vec3 textureUV[100];

    TextRenderer();
    void Load();
    void LoadOffsets();
    void SetText(std::string text);
    bool HasChanged();
    size_t GetLength();
private:
    std::string str;
    bool changed;
};

}