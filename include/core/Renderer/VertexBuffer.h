#pragma once

#include "glad/glad.h"
#include "glfw/glfw3.h"

#include "core/Logger/Logger.h"

namespace dem{
namespace Renderer{

    class VertexBuffer{
    public:
        GLuint gl;
        float* verticies;
        size_t size;
        VertexBuffer(float* verticies, size_t size);
    };
}
}