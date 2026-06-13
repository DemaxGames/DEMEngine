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
    std::vector<math::vec3> verticies_set;
    std::vector<math::vec3> normals_set;
    std::vector<math::vec2> uv_set;
    std::vector<size_t> vertex_indicies;
    std::vector<size_t> uv_indicies;
    std::vector<size_t> normal_indicies;


    int Load(std::string path);
private:
    void LoadFromObj(std::string path);
};

}

