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
    GLenum slot;

    GLImage(Image* image_ptr = NULL);
    void Load();
};
}
}