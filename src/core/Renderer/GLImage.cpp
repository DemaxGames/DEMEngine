#include "core/Renderer/GLImage.h"

dem::Renderer::GLImage::GLImage(dem::Image* image_ptr){
    image = image_ptr;

    glGenTextures(1, &gl);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gl);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 1, GL_RGBA, GL_UNSIGNED_BYTE, image->data);

    glBindTexture(GL_TEXTURE_2D, 0);
    return;
}