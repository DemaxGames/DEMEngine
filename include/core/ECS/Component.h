#pragma once
#include <vector>
#include "core/math/uid.h"

namespace dem{
namespace ecs{

class IComponentPool{
    
};

template <class T>
class ComponentPool : public IComponentPool{
public:
    std::map<math::uid_t, T> componentMap;
};

}
}