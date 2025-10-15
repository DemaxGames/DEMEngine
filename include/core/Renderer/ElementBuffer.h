#pragma once

#include "glad/glad.h"
#include "glfw/glfw3.h"

#include "core/Logger/Logger.h"
#include "core/math/vec.h"

namespace dem{
namespace Renderer{

    class ElementBuffer{
    public:
        GLuint gl;
        unsigned int* indicies;
        size_t size;
        ElementBuffer();
        ElementBuffer(unsigned int* indicies, size_t size);
    };
}
}