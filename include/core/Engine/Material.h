#pragma once
#include "core/Renderer/Shader.h"
#include "core/Renderer/GLProgram.h"
#include "core/Renderer/GLImage.h"
#include "core/Engine/Image.h"

namespace dem{
    
class Material{
public:
    Renderer::Shader vertex;
    Renderer::Shader fragment;
    Renderer::GLProgram program;
    
    Image* image;
    Renderer::GLImage glImage;
    

    GLint vec3_vpos_location;
    GLint vec3_normal_location;
    GLint vec2_uv_location;
    GLint mat4_projection_location;
    GLint mat4_view_location;
    GLint mat4_model_location;
    GLint float_time_location;
    GLint sampler2D_tex;

    Material();
    void Load();
};

}