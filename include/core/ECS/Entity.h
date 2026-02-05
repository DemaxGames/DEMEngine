#pragma once
#include "core/math/uid.h"
#include "core/ECS/Component.h"
#include "core/Logger/Logger.h"


namespace dem{
namespace ecs{

class Entity{
public:
    math::uid_t id;

    template<class T> void AddComponent();
    template<class T> T* GetComponent();
};

}
}

#include "core/ECS/ecs.h"

namespace dem{
namespace ecs{

extern std::map<std::type_index, IComponentPool*> componentPools;

template<class T> void Entity::AddComponent(){
    IComponentPool *genericComponentPool;
    if(componentPools.find(typeid(T)) != componentPools.end()){
        genericComponentPool = componentPools[typeid(T)];
    }
    else{
        Logger::get()->log("ERROR: component is not registered!!!");
        return;
    }


    T component;
    ComponentPool<T>* componentPool;
    if(!(componentPool = dynamic_cast<ComponentPool<T>*>(genericComponentPool))){
        Logger::get()->log("ERROR cannot cast generic component pool");
        return; 
    }
    componentPool->componentMap.emplace(id, component);

}

template<class T> T* Entity::GetComponent(){
    IComponentPool *genericComponentPool;
    if(componentPools.find(typeid(T)) != componentPools.end()){
        genericComponentPool = componentPools[typeid(T)];
    }
    else{
        Logger::get()->log("ERROR: component is not registered!!!");
        return nullptr;
    }

    T* component;
    ComponentPool<T>* componentPool = dynamic_cast<ComponentPool<T>*>(genericComponentPool);
    if(componentPool->componentMap.find(id) != componentPool->componentMap.end()){
        component = &componentPool->componentMap[id];
    }
    else{
        Logger::get()->log("ERROR: cannot find component in entity");
        return nullptr;
    }
    return component;
}

}
}
