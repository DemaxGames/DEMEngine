#pragma once
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "core/Engine/Image.h"

namespace dem{
namespace Renderer{

class GLImage{
public:
    GLuint gl;
    Image* image;

    GLImage(Image* image_ptr = NULL);
};
}
}