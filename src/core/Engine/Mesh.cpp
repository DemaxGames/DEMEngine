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
        std::string vertex_str;
        switch (line[0]){
            case 'o':
                for(; line[j] != ' ' && line[j] != '\0'; j++){
                    name += line[j];
                }
                break;
            case 'v':
                for(; j < line.length(); j++){
                    if(line[j] == ' ' || j == line.length() - 1){
                        vertex_str += line[j];
                        verticies_vector.push_back(std::stof(vertex_str));
                        vertex_str.clear();
                    } else vertex_str += line[j];
                }
                break;
            case 'f':
                for(; j < line.length(); j++){
                    if(line[j] == ' ' || j == line.length() - 1){
                        index_str += line[j];
                        indicies_vector.push_back(std::stoul(index_str) - 1);
                        index_str.clear();
                    } else index_str += line[j];
                }
                break;
        }
    }

    verticies_count = verticies_vector.size() / 3;
    verticies = verticies_vector.data();

    

    indicies_count = indicies_vector.size() / 3;
    indicies = indicies_vector.data();
}

}

