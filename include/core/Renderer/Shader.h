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

    Shader();
    void Create(GLenum shaderType = GL_VERTEX_SHADER, std::string sourcePath = "");
    int Compile();
};
}
}