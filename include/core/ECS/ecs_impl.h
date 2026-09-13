#pragma once
#include "core/Logger/Logger.h"

namespace dem{
namespace ecs{

template<class T> int RegisterComponent(){
    ComponentPool<T>* componentPool = new ComponentPool<T>();
    componentPools.emplace(typeid(T), componentPool);

    Logger::get()->log("Component Registered: " + std::string(typeid(T).name()));

    return 0;
}

}
}