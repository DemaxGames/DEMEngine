#pragma once

#include <string>
#include "core/Renderer/VertexArray.h"
#include "core/Components/TransformComponent.h"

namespace dem{

class TextRenderer{
public:
    TransformComponent* transform;
    Renderer::VertexArray VAO;
    std::string str;
    math::vec2 offsets[100];
    int data[100];

    void Load();
};

}