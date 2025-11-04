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
    float* data;
    size_t data_size;

    float* verticies;
    size_t verticies_size;

    float* normals;
    size_t normals_size;

    VertexBuffer();
    VertexBuffer(float* verticies, size_t size);
    VertexBuffer(math::vec3* verticies, size_t size);
};

}
}
