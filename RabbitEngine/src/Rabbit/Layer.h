#pragma once

#include "Rabbit/Core.h"
#include "Rabbit/Events/Event.h"

namespace Rabbit {
    
    class RABBIT_API Layer
    {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& e) {}

        inline const std::string& GetName() const { return m_DebugName; }

    protected:
        std::string m_DebugName;
    };
}