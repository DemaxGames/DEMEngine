#pragma once
#include <map>
#include <string>
#include <typeindex>
#include "core/ECS/Component.h"

namespace dem{
namespace ecs{
extern std::map<std::type_index, IComponentPool> componentPools;

template<class T> int RegisterComponent();

}
}

#include "ecs_impl.h"