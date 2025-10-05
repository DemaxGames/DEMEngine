#pragma once

#include "glad/glad.h"
#include "glfw/glfw3.h"

#include "core/Logger/Logger.h"
#include "core/math/vec.h"

namespace dem{
namespace Renderer{

    class VertexBuffer{
    public:
        GLuint gl;
        math::vec3* verticies;
        size_t size;
        VertexBuffer(float* verticies, size_t size);
        VertexBuffer(math::vec3* verticies, size_t size);
    };
}
}