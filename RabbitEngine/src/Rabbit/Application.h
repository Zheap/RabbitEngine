#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace Rabbit {

    class RABBIT_API Application
    {
    public:
        Application();
        virtual ~Application();
        
        void Run();
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}

