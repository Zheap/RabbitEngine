#pragma once

#include "entt.hpp"

#include "Rabbit/Core/Timestep.h"

namespace Rabbit {

    class Scene
    {
    public: 
        Scene();
        ~Scene();

        entt::entity CreateEntity();

        // Temp
        entt::registry& Reg() { return m_Registry; }

        void OnUpdate(Timestep ts);

    private:
        entt::registry m_Registry;
    };
}