#include "core/ECS/Entity.h"
#include "core/ECS/ecs.h"
#include "core/Logger/Logger.h"

namespace dem{
namespace ecs{


template<class T> void Entity::AddComponent(const T component){
    

}

template<class T> T* Entity::GetComponent(){
    IComponentPool *genericComponentPool;
    if(componentPools.find(typeid(T)) != componentPools.end()){
        genericComponentPool = &componentPools[typeid(T)];
    }
    else{
        Logger::get()->log("ERROR: component is not registered!!!");
        return nullptr;
    }

    T* component;
    ComponentPool<T>* componentPool = dynamic_cast<ComponentPool<T>*>(genericComponentPool);
    if(componentPool.find(id) != componentPool.end()){
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
