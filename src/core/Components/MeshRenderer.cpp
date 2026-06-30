#include "core/Components/MeshRenderer.h"

namespace dem{

void MeshRenderer::LoadVBO(){
    VAO.VBO.data = new float[mesh->vertex_indicies.size() * (3 + 3 + 2)];
    for(int i = 0; i < mesh->vertex_indicies.size(); i++){
        VAO.VBO.data[i * (3 + 3 + 2) + 0] = mesh->verticies_set[mesh->vertex_indicies[i]].data[0];
        VAO.VBO.data[i * (3 + 3 + 2) + 1] = mesh->verticies_set[mesh->vertex_indicies[i]].data[1];
        VAO.VBO.data[i * (3 + 3 + 2) + 2] = mesh->verticies_set[mesh->vertex_indicies[i]].data[2];
        VAO.VBO.data[i * (3 + 3 + 2) + 3] = mesh->normals_set[mesh->normal_indicies[i]].data[0];
        VAO.VBO.data[i * (3 + 3 + 2) + 4] = mesh->normals_set[mesh->normal_indicies[i]].data[1];
        VAO.VBO.data[i * (3 + 3 + 2) + 5] = mesh->normals_set[mesh->normal_indicies[i]].data[2];
        VAO.VBO.data[i * (3 + 3 + 2) + 6] = mesh->uv_set[mesh->uv_indicies[i]].data[0];
        VAO.VBO.data[i * (3 + 3 + 2) + 7] = mesh->uv_set[mesh->uv_indicies[i]].data[1];
        //std::cout << mesh->uv_set[mesh->uv_indicies[i]].data[0] << " " << mesh->uv_set[mesh->uv_indicies[i]].data[1] << "\n";
    }

    VAO.VBO.data_size = mesh->vertex_indicies.size() * (3 + 3 + 2);
    VAO.VBO.LoadData();
}

void MeshRenderer::Load(){
    VAO.material = material;
    LoadVBO();
    VAO.BindAll();

    return;
}



}