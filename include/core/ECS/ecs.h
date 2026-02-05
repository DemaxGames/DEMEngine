#pragma once
#include <map>
#include <string>
#include <typeindex>
#include "core/ECS/Component.h"
#include "core/ECS/Entity.h"

namespace dem{
namespace ecs{
extern std::map<std::type_index, IComponentPool*> componentPools;

void init();

template<class T> int RegisterComponent();
Entity CreateEntity();

}
}

#include "ecs_impl.h"