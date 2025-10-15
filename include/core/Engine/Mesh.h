#pragma once
#include <vector>
#include <string>

#include "core/math/vec.h"
#include "core/Renderer/VertexArray.h"

namespace dem
{
class Mesh{
public:
    std::string name;
    std::vector<float> verticies_vector;
    std::vector<unsigned int> indicies_vector;

    float* verticies;
    size_t verticies_count;

    bool drawElementsSupport;
    unsigned int* indicies;
    size_t indicies_count;

    Renderer::VertexArray VAO;

    Mesh(float* pVerticies = NULL, size_t vertexCount = 0,
        unsigned int* pIndicies = NULL, size_t indexCount = 0);
    int Load(std::string path);
private:
    void LoadFromObj(std::string path);
};
}

