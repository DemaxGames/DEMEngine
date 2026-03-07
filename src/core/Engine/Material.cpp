#include "core/Engine/Material.h"

#include "core/Logger/Logger.h"

namespace dem{

Material::Material(){
    fragment.sourceFile = "examples/shader.frag";
    vertex.sourceFile = "examples/shader.vert";
}


void Material::Load(){
    Logger::get()->log("Loading material");
    Logger* logger = Logger::get();

    vertex.Create(GL_VERTEX_SHADER, vertex.sourceFile);
    fragment.Create(GL_FRAGMENT_SHADER, fragment.sourceFile);

    if(!vertex.compiled){
        if(vertex.Compile() != 0) return;
    }
    if(!fragment.compiled){
        if(fragment.Compile() != 0) return;
    }

    program.vertex = &vertex;
    program.fragment = &fragment;
    if(program.Link() != 0) return;
    
    glImage.Load();

    vec3_vpos_location = glGetAttribLocation(program.gl, "vPos");
    vec3_normal_location = glGetAttribLocation(program.gl, "normal");
    vec2_uv_location = glGetAttribLocation(program.gl, "uv");
    mat4_model_location = glGetUniformLocation(program.gl, "model");
    mat4_view_location = glGetUniformLocation(program.gl, "view");
    mat4_projection_location = glGetUniformLocation(program.gl, "projection");
    float_time_location = glGetUniformLocation(program.gl, "time");
    sampler2D_tex = glGetUniformLocation(program.gl, "tex");
}

}