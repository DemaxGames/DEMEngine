#include "core/Renderer/Shader.h"

namespace dem{

Renderer::Shader::Shader(){
    compiled = false;
}

void Renderer::Shader::Create(GLenum shaderType, std::string sourcePath){
    Logger::get()->log("Creating Shader");
    if(!sourcePath.empty()) sourceFile = sourcePath;
    

    if(shaderType != GL_SHADER_TYPE){
        type = shaderType;
        gl = glCreateShader(type);
    }
}


int Renderer::Shader::Compile(){
    Logger::get()->log("Compiling shader " + sourceFile);

    FILE* file = fopen(sourceFile.data(), "r");
    if(file == NULL){
        Logger::get()->log("ERROR: cannot find file:");
        Logger::get()->log(sourceFile);
        return -1;
    }
    char* src = new char[std::filesystem::file_size(sourceFile) + 1];
    if(!fread(src, std::filesystem::file_size(sourceFile), 1, file)){
        Logger::get()->log("ERROR: could not read from file: " + sourceFile);
        return -1;
    }
    src[std::filesystem::file_size(sourceFile)] = '\0';
    glShaderSource(gl, 1, &src, NULL);
    glCompileShader(gl);
    GLint status;
    glGetShaderiv(gl, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE){
        Logger::get()->log("ERROR: compilation failed in file " + sourceFile);
        GLint log_length;
        glGetShaderiv(gl, GL_INFO_LOG_LENGTH, &log_length);
        char* log = new char[log_length];
        glGetShaderInfoLog(gl, log_length, NULL, log);
        Logger::get()->log(log);
        return -1;
    }
    compiled = true;

    return 0;
}
}