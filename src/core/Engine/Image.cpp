#include "core/Engine/Image.h"
#include "core/Logger/Logger.h"

int dem::Image::Load(std::string filepath){
    dem::Logger::get()->log("Loading image from " + filepath);
    data = stbi_load(filepath.data(), &width, &height, &colorChannels, 0);
    
    return 0;
}