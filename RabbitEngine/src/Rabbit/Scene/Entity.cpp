#include "rbpch.h"
#include "Entity.h"

namespace Rabbit {

    Entity::Entity(entt::entity handle, Scene* scene)
        : m_EntityHandle(handle), m_Scene(scene)
    {

    }

}