#include "core/Components/MeshRenderer.h"

namespace dem{

void MeshRenderer::Load(){
    mesh->VAO.material = material;
    mesh->VAO.VBO = mesh->GetVBO();
    mesh->VAO.BindAll();

    return;
}

}