#pragma once
#include <vector>
#include <map>
#include "core/math/uid.h"

namespace dem{
namespace ecs{

class IComponentPool{
public:
    virtual ~IComponentPool() = default;
};

template <class T>
class ComponentPool : public IComponentPool{
public:
    std::map<math::uid_t, T> componentMap;
    ComponentPool(){
        componentMap = std::map<math::uid_t, T>();
    }
    ~ComponentPool() = default;
};

}
}