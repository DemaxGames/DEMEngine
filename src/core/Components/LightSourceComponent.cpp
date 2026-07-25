#include "core/Components/LightSourceComponent.h"

namespace dem{

LightSourceComponent::LightSourceComponent(){
    transform = nullptr;
    color = math::vec3(1.f, 1.f, 1.f);
    strength = 1.f;
}

}