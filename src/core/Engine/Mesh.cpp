#include "core/Engine/Mesh.h"

#include "core/Logger/Logger.h"


#define VECTOR_RESERVE 3


namespace dem{

Mesh::Mesh(float* pVerticies, size_t vertexCount,
            unsigned int* pIndicies, size_t indexCount){
    if(vertexCount > 0 && pVerticies != NULL){
        verticies = pVerticies;
        verticies_count = vertexCount;
    }
    else{
        verticies = NULL;
        verticies_count = 0;
    }

    if(indexCount > 0 && pIndicies != NULL){
        drawElementsSupport = true;
        indicies = pIndicies;
        indicies_count = indexCount;
    }
    else{
        drawElementsSupport = false;
        indicies = NULL;
        indicies_count = 0;
    }

    smoothShading = false;
}

int Mesh::Load(std::string path){
    std::string log_str;
    log_str = "Loading mesh from ";
    log_str += path;
    Logger::get()->log(log_str);

    if((int)path.find(".obj") > 1){
        drawElementsSupport = true;
        LoadFromObj(path);
    }
    else{
        log_str = "ERROR: Cannot load mesh from file with extension ";
        log_str += path.substr(path.find("."), 10);
        Logger::get()->log(log_str);
        return -1;
    }
    return 0;
}

void Mesh::LoadFromObj(std::string path){
    verticies_vector.reserve(VECTOR_RESERVE);
    indicies_vector.reserve(VECTOR_RESERVE);
    bool normalsEnabled = false;
    
    std::string line;
    std::ifstream file(path);
    if(!file){
        std::string log_str;
        log_str = "ERROR: Cannot find ";
        log_str += path;
        Logger::get()->log(log_str);
    }

    while(std::getline(file, line)){
        int j = 2;
        std::string index_str;
        std::string normal_str;
        std::string vertex_str;

        bool skipIndex = false;
        switch (line[0]){
            case 'o':
                for(; line[j] != ' ' && line[j] != '\0'; j++){
                    name += line[j];
                }
                break;
            case 'v':
                if(line[1] == 'n'){
                    if(!normalsEnabled) normalsEnabled = true;
                } else{
                    for(; j < line.length(); j++){
                        if(line[j] == ' ' || j == line.length() - 1){
                            vertex_str += line[j];
                            verticies_vector.push_back(std::stof(vertex_str));
                            vertex_str.clear();
                        } else vertex_str += line[j];
                    }
                }
                break;
            case 'f':
                if(normalsEnabled){
                    for(; j < line.length(); j++){
                        if(line[j] == '/'){
                            for(;line[j] != ' '; j++);
                        }
                        if(line[j] == ' ' || j == line.length() - 1){
                            index_str += line[j];
                            indicies_vector.push_back(std::stoul(index_str) - 1);
                            index_str.clear();
                        } else index_str += line[j];
                    }
                } else{
                    for(; j < line.length(); j++){
                        if(line[j] == ' ' || j == line.length() - 1){
                            index_str += line[j];
                            indicies_vector.push_back(std::stoul(index_str) - 1);
                            index_str.clear();
                        } else index_str += line[j];
                    }
                }
                break;
        }
    }

    verticies_count = verticies_vector.size() / 3;
    verticies = verticies_vector.data();

    indicies_count = indicies_vector.size() / 3;
    indicies = indicies_vector.data();
}

Renderer::VertexBuffer Mesh::GetVBO(){
    Renderer::VertexBuffer VBO;
    glGenBuffers(1, &VBO.gl);

    if(smoothShading){
        
    } else{
        VBO.verticies = new float[indicies_count * 9];
        VBO.normals = new float[indicies_count * 9];

        for(int i = 0; i < indicies_count; i++){
            VBO.verticies[i * 9 + 0] = verticies[indicies[i * 3 + 0] * 3 + 0];
            VBO.verticies[i * 9 + 1] = verticies[indicies[i * 3 + 0] * 3 + 1];
            VBO.verticies[i * 9 + 2] = verticies[indicies[i * 3 + 0] * 3 + 2];
            VBO.verticies[i * 9 + 3] = verticies[indicies[i * 3 + 1] * 3 + 0];
            VBO.verticies[i * 9 + 4] = verticies[indicies[i * 3 + 1] * 3 + 1];
            VBO.verticies[i * 9 + 5] = verticies[indicies[i * 3 + 1] * 3 + 2];
            VBO.verticies[i * 9 + 6] = verticies[indicies[i * 3 + 2] * 3 + 0];
            VBO.verticies[i * 9 + 7] = verticies[indicies[i * 3 + 2] * 3 + 1];
            VBO.verticies[i * 9 + 8] = verticies[indicies[i * 3 + 2] * 3 + 2];
        }
        VBO.verticies_size = indicies_count * 9;

        for(int i = 0; i < indicies_count; i++){

            math::vec3 p1(VBO.verticies[i*9 + 0], VBO.verticies[i*9 + 1], VBO.verticies[i*9 + 2]);
            math::vec3 p2(VBO.verticies[i*9 + 3], VBO.verticies[i*9 + 4], VBO.verticies[i*9 + 5]);
            math::vec3 p3(VBO.verticies[i*9 + 6], VBO.verticies[i*9 + 7], VBO.verticies[i*9 + 8]);

            math::vec3 a = p2 - p1;
            math::vec3 b = p3 - p1;

            math::vec3 n = math::normal(a, b);

            VBO.normals[i * 9 + 0] = n[0];
            VBO.normals[i * 9 + 1] = n[1];
            VBO.normals[i * 9 + 2] = n[2];
            VBO.normals[i * 9 + 3] = n[0];
            VBO.normals[i * 9 + 4] = n[1];
            VBO.normals[i * 9 + 5] = n[2];
            VBO.normals[i * 9 + 6] = n[0];
            VBO.normals[i * 9 + 7] = n[1];
            VBO.normals[i * 9 + 8] = n[2];
        }

        VBO.normals_size = indicies_count * 9;

        VBO.data = new float[indicies_count * 18];
        
        for(int i = 0; i < indicies_count; i++){
            VBO.data[i * 18 + 0] = VBO.verticies[i * 9 + 0];
            VBO.data[i * 18 + 1] = VBO.verticies[i * 9 + 1];
            VBO.data[i * 18 + 2] = VBO.verticies[i * 9 + 2];
            VBO.data[i * 18 + 3] = VBO.normals[i * 9 + 0];
            VBO.data[i * 18 + 4] = VBO.normals[i * 9 + 1];
            VBO.data[i * 18 + 5] = VBO.normals[i * 9 + 2];
            VBO.data[i * 18 + 6] = VBO.verticies[i * 9 + 3];
            VBO.data[i * 18 + 7] = VBO.verticies[i * 9 + 4];
            VBO.data[i * 18 + 8] = VBO.verticies[i * 9 + 5];
            VBO.data[i * 18 + 9] = VBO.normals[i * 9 + 3];
            VBO.data[i * 18 + 10] = VBO.normals[i * 9 + 4];
            VBO.data[i * 18 + 11] = VBO.normals[i * 9 + 5];
            VBO.data[i * 18 + 12] = VBO.verticies[i * 9 + 6];
            VBO.data[i * 18 + 13] = VBO.verticies[i * 9 + 7];
            VBO.data[i * 18 + 14] = VBO.verticies[i * 9 + 8];
            VBO.data[i * 18 + 15] = VBO.normals[i * 9 + 6];
            VBO.data[i * 18 + 16] = VBO.normals[i * 9 + 7];
            VBO.data[i * 18 + 17] = VBO.normals[i * 9 + 8];
        }

        VBO.data_size = indicies_count * 18;

        glBindBuffer(GL_ARRAY_BUFFER, VBO.gl);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * VBO.data_size, (void*)VBO.data, GL_STATIC_DRAW);
    }

    return VBO;
}

}

