#include "core/Components/MeshRenderer.h"

namespace dem{

void MeshRenderer::Load(){
    mesh->VAO.program = program;
    mesh->VAO.VBO = mesh->GetVBO();
    mesh->VAO.BindAll();

    return;
}

}