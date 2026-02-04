#pragma once
#include "core/math/uid.h"

namespace dem{
namespace ecs{

class Entity{
public:
    math::uid_t id;

    template<class T> void AddComponent(T component);
    template<class T> T* GetComponent();
};

}
}