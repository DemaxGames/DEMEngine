#pragma once

#include "core/Engine/Mesh.h"
#include "core/Renderer/GLProgram.h"

namespace dem{

class MeshRenderer{
public:
    Renderer::GLProgram* program;
    Mesh* mesh;
    
    void Load();
};

}