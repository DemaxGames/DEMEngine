#pragma once

#include "core/Renderer/VertexBuffer.h"
#include "core/Renderer/GLImage.h"
#include "core/Renderer/GLProgram.h"

namespace dem{
namespace Renderer{

extern GLProgram* sharedProgram;
extern VertexBuffer* sharedVBO;
extern GLImage* sharedGLImage;

extern GLint offsets_location;
extern GLint textureUV_location;
extern GLint sharedTex_location;

void TextInit();

}
}