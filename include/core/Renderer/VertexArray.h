#pragma once

#include "glad/glad.h"
#include "glfw/glfw3.h"

#include "core/Logger/Logger.h"
#include "core/Engine/Material.h"
#include "core/Renderer/ElementBuffer.h"
#include "core/Renderer/VertexBuffer.h"

namespace dem{
namespace Renderer{
    
    class VertexArray{
    public:
        GLuint gl;
        Material* material;
        ElementBuffer EBO;
        VertexBuffer VBO;

        VertexArray();
        int BindAll();
    };
}
}
