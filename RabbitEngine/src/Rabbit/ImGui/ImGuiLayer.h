#pragma once

#include "Rabbit/Layer.h"

#include "Rabbit/Events/ApplicationEvent.h"
#include "Rabbit/Events/KeyEvent.h"
#include "Rabbit/Events/MouseEvent.h"

namespace Rabbit {

    class RABBIT_API ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnImGuiRender() override;

        void Begin();
        void End();

    private:
        float m_Time = 0.0f;
    };
}