#pragma once

#include <vector>
#include "core/Engine/Scene.h"
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "core/Logger/Logger.h"
#include "core/Renderer/GLProgram.h"
#include "core/Renderer/Shader.h"
#include "core/Renderer/VertexArray.h"
#include "core/Renderer/VertexBuffer.h"

namespace dem{
namespace Renderer{
GLFWwindow* window;
dem::Renderer::GLProgram* program;
dem::Renderer::VertexArray* vertexArray;
std::vector<dem::Renderer::VertexBuffer> vertexBuffers;


int Init(int width, int height);
int LoadScene(dem::Scene scene);
int Render();


}
}