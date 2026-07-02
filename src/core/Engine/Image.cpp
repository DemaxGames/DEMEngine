#include "core/Engine/Image.h"
#include "core/Logger/Logger.h"

int dem::Image::Load(std::string filepath){
    src = filepath;
    dem::Logger::get()->log("Loading image from " + filepath);
    data = stbi_load(filepath.data(), &width, &height, &colorChannels, 0);
    if(data == nullptr){
        dem::Logger::get()->log("ERROR: failed to load " + filepath);
        return -1;
    }
    
    return 0;
}