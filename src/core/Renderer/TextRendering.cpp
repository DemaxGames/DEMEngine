#include "core/Renderer/TextRendering.h"

namespace dem{
namespace Renderer{

GLProgram* sharedProgram;
VertexBuffer* sharedVBO;
GLImage* sharedGLImage;
GLint offsets_location;
GLint textureUV_location;
GLint sharedTex_location;

float quadVert[] = {
    0.0f,  0.0f,
    1.0f,  0.0f,
    1.0f,  1.0f,

    0.0f,  0.0f,
    0.0f,  1.0f,
    1.0f,  1.0f
};  

void TextInit(){
    sharedVBO = new VertexBuffer(quadVert, sizeof(quadVert) / sizeof(float));
    Image* img = new Image;
    img->Load("examples/font.png");
    sharedGLImage = new GLImage(img);
    sharedGLImage->Load();
    Shader* vertexShader = new Shader();
    vertexShader->Create(GL_VERTEX_SHADER, "examples/text.vert");
    vertexShader->Compile();
    Shader* fragmentShader = new Shader();
    fragmentShader->Create(GL_FRAGMENT_SHADER, "examples/text.frag");
    fragmentShader->Compile();
    sharedProgram = new GLProgram(vertexShader, fragmentShader);
    sharedProgram->Link();

    offsets_location = glGetUniformLocation(Renderer::sharedProgram->gl, "offsets");
    textureUV_location = glGetUniformLocation(Renderer::sharedProgram->gl, "textureUV");
    sharedTex_location = glGetUniformLocation(Renderer::sharedProgram->gl, "tex");
}

}
}