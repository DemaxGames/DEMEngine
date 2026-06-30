#include "core/Engine/Mesh.h"

#include "core/Logger/Logger.h"


#define VECTOR_RESERVE 3

struct indexVec3{
    int vertex;
    int uv;
    int normal;
};

struct faceIndicies{
    indexVec3 data[3];
};

namespace dem{

math::vec3 GetVec3FromString(std::string line){
    math::vec3 vec;
    int i = 0;

    while(i < line.length()){
        if((line[i] >= '0' && line[i] <= '9') || line[i] == '-'){
            break;
        }

        i++;
    }

    for(int j; j < 3; j++){
        std::string str;
        for(; line[i] != ' '; i++) str += line[i];
        vec[j] = std::stof(str);
        str.clear();
        i++;
    }

    //std::cout << vec[0] << " " << vec[1] << " " << vec[2] << "\n";
    return vec;
}

math::vec2 GetVec2FromString(std::string line){
    math::vec2 vec;
    int i = 0;

    while(i < line.length()){
        if((line[i] >= '0' && line[i] <= '9') || line[i] == '-'){
            break;
        }

        i++;
    }

    for(int j = 0; j < 2; j++){
        std::string str;
        for(; line[i] != ' '; i++) str += line[i];
        vec[j] = std::stof(str);
        str.clear();
        i++;
    }
    return vec;
}

faceIndicies GetIndiciesFromString(std::string line){
    faceIndicies face;
    int i = 0;

    while(i < line.length()){
        if((line[i] >= '0' && line[i] <= '9') || line[i] == '-'){
            break;
        }

        i++;
    }

    for(int j = 0; j < 3; j++){
        std::string str;
        indexVec3 vec;
        for(; line[i] != ' ' && line[i] != '/'; i++) str += line[i];
        vec.vertex = std::stoi(str) - 1;
        str.clear();
        i++;

        for(; line[i] != ' ' && line[i] != '/'; i++) str += line[i];
        vec.uv = std::stoi(str) - 1;
        str.clear();
        i++;

        for(; line[i] != ' ' && line[i] != '/'; i++) str += line[i];
        vec.normal = std::stoi(str) - 1;
        str.clear();
        i++;
        face.data[j] = vec;
        //std::cout << vec.vertex << " " << vec.uv << " "  << vec.normal << "\n";
    }

    return face;
}



int Mesh::Load(std::string path){
    std::string log_str;
    log_str = "Loading mesh from ";
    log_str += path;
    Logger::get()->log(log_str);

    if((int)path.find(".obj") > 1){
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
    verticies_set.reserve(VECTOR_RESERVE);
    vertex_indicies.reserve(VECTOR_RESERVE);
    
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
        //std::cout << line << "\n";
        switch(line[0]){
            case 'v':
                switch (line[1]){
                    case ' ':
                        verticies_set.push_back(GetVec3FromString(line));
                        break;
                    case 'n':
                        normals_set.push_back(GetVec3FromString(line));
                        break;
                    case 't':
                        uv_set.push_back(GetVec2FromString(line));
                        break;
                    default:
                        break;
                }
                break;
            case 'f':
                {
                faceIndicies face = GetIndiciesFromString(line);
                for(int i = 0; i < 3; i++){
                    vertex_indicies.push_back(face.data[i].vertex);
                    uv_indicies.push_back(face.data[i].uv);
                    normal_indicies.push_back(face.data[i].normal);
                }
                }
                break;

        }
    }
}



}


