#pragma once
#include "stb/stb_image.h"
#include <string>

namespace dem{

class Image {
public:
    std::string src;
    int width, height, colorChannels;
    unsigned char* data;

    int Load(std::string filepath);
};
}