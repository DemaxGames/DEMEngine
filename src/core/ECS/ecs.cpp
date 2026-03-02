#include "core/ECS/ecs.h"
#include "core/math/uid.h"

namespace dem{
namespace ecs{

std::map<std::type_index, IComponentPool*> componentPools;
std::vector<Entity> entities;

void init(){
    componentPools = std::map<std::type_index, IComponentPool*>();
    entities = std::vector<Entity>();
}

Entity CreateEntity(){
    Entity entity{.id = math::GetUID()};
    entities.push_back(entity);
    return entity;
}

}
}