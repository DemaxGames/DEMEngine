#pragma once

#include <vector>
#include "core/Engine/Image.h"
#include "core/Engine/Mesh.h"
#include "core/Components/CameraComponent.h"
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "core/Logger/Logger.h"
#include "core/Renderer/GLProgram.h"
#include "core/Renderer/GLImage.h"
#include "core/Renderer/Shader.h"
#include "core/Renderer/VertexArray.h"
#include "core/Renderer/VertexBuffer.h"
#include "core/Renderer/ElementBuffer.h"
#include "core/math/vec.h"

namespace dem{
namespace Renderer{
extern GLFWwindow* window;

int Init(int width, int height);
int LoadScene();
int Render();

}
}