#pragma once

#include "glad/glad.h"
#include "glfw/glfw3.h"

#include "core/Logger/Logger.h"
#include "core/Renderer/GLProgram.h"

namespace dem{
namespace Renderer{

    class VertexArray{
    public:
        GLuint gl;
        VertexArray();
        int CreateVertexAttribPointer(GLProgram program);
    };
}
}