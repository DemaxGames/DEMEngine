#pragma once

#include "core/Engine/Mesh.h"
#include "core/Engine/Material.h"

namespace dem{

class MeshRenderer{
public:
    Material* material;
    Mesh* mesh;
    
    void Load();
};

}