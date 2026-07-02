#include "core/Renderer/GLImage.h"
#include "core/Logger/Logger.h"

int textureSlotCount = 0;

dem::Renderer::GLImage::GLImage(dem::Image* image_ptr){
    if(image_ptr == NULL) return;
    image = image_ptr;
    return;
}

void dem::Renderer::GLImage::Load(){
    if(image == NULL) {
        dem::Logger::get()->log("ERROR: cannot load GLImage, the image is NULL");
        return;
    }
    Logger::get()->log("Loading image to the GPU memory: " + image->src);
    Logger::get()->log("Textures color channels: ", image->colorChannels);

    glGenTextures(1, &gl);
    slot = GL_TEXTURE0 + textureSlotCount;
    Logger::get()->log("Used slot: ", textureSlotCount);
    textureSlotCount++;
    glBindTexture(GL_TEXTURE_2D, gl);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLenum format;
    switch(image->colorChannels){
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            format = 0;
            break;
    }

    if(format == 0){
        Logger::get()->log("ERROR: cannot load with image's color channels: ", image->colorChannels);
        return;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, format, GL_UNSIGNED_BYTE, image->data);

    glBindTexture(GL_TEXTURE_2D, 0);
    return;
}