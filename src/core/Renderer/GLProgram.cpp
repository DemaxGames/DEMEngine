#include "core/Renderer/GLProgram.h"

namespace dem{

Renderer::GLProgram::GLProgram(){
    Logger::get()->log("Creating program");

    gl = glCreateProgram();
    linked = false;
}

Renderer::GLProgram::GLProgram(Shader* vertexShader, Shader* fragmentShader){
    Logger::get()->log("Creating program");
    
    gl = glCreateProgram();
    linked = false;
    vertex = vertexShader;
    fragment = fragmentShader;
}

int Renderer::GLProgram::Link(){
    Logger::get()->log("Linking program");
    if(vertex->compiled == true) glAttachShader(gl, vertex->gl);
    else Logger::get()->log("ERROR: vertex shader is not compiled, but you trying to link the program with him");
    if(fragment->compiled == true) glAttachShader(gl, fragment->gl);
    else Logger::get()->log("ERROR: fragment shader is not compiled, but you trying to link the program with him");
    glLinkProgram(gl);
    linked = true;

    vec3_vpos_location = glGetAttribLocation(gl, "vPos");
    mat4_model_location = glGetUniformLocation(gl, "model");
    mat4_view_location = glGetUniformLocation(gl, "view");
    mat4_projection_location = glGetUniformLocation(gl, "projection");
    float_time_location = glGetUniformLocation(gl, "time");

    return 0;
}
}