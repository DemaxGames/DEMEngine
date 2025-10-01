#include "core/Renderer/GLProgram.h"

dem::Renderer::GLProgram::GLProgram(){
    Logger::get()->log("Creating program");

    gl = glCreateProgram();
    linked = false;
}

dem::Renderer::GLProgram::GLProgram(Shader* vertexShader, Shader* fragmentShader){
    Logger::get()->log("Creating program");
    
    gl = glCreateProgram();
    linked = false;
    vertex = vertexShader;
    fragment = fragmentShader;
}

int dem::Renderer::GLProgram::Link(){
    Logger::get()->log("Linking program");
    if(vertex->compiled == true) glAttachShader(gl, vertex->gl);
    else Logger::get()->log("ERROR: vertex shader is not compiled, but you trying to link the program with him");
    if(fragment->compiled == true) glAttachShader(gl, fragment->gl);
    else Logger::get()->log("ERROR: fragment shader is not compiled, but you trying to link the program with him");
    glLinkProgram(gl);
    linked = true;

    return 0;
}