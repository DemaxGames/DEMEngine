#pragma once
#include <vector>
#include "core/Engine/Object.h"
#include "core/Engine/Camera.h"

namespace dem{

class Scene{
public:
    std::vector<Object> objects;
    Camera* camera;
};
}