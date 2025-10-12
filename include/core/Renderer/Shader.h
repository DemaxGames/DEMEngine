#pragma once

#include <string>
#include <filesystem>
#include "core/Logger/Logger.h"
#include "glad/glad.h"
#include "glfw/glfw3.h"


namespace dem{
namespace Renderer{

class Shader{
public:
    GLuint gl;
    GLenum type;
    std::string sourceFile;
    bool compiled;

    Shader(const char* string, GLenum shaderType);
    int Compile();
};
}
}