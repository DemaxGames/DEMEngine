#pragma once

#include "core/Renderer/Shader.h"
#include "core/Logger/Logger.h"
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include <string>


namespace dem{
namespace Renderer{

class GLProgram{
public:
    GLuint gl;
    Shader* vertex;
    Shader* fragment;
    bool linked;

    GLProgram();
    GLProgram(Shader* vertexShader, Shader* fragmentShader);
    int Link();
};
}
}