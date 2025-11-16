#include "core/Engine/Image.h"

int dem::Image::LoadImage(std::string filepath){
    data = stbi_load(filepath.data(), &width, &height, &colorChannels, 0);
    
    return 0;
}