#pragma once

#include "Rabbit/Core/Layer.h"

#include "Rabbit/Events/ApplicationEvent.h"
#include "Rabbit/Events/KeyEvent.h"
#include "Rabbit/Events/MouseEvent.h"

namespace Rabbit {

    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnImGuiRender() override;
        virtual void OnEvent(Event& e) override;

        void Begin();
        void End();

        void BlockEvents(bool block) { m_BlockEvents = block; }

    private:
        void SetDarkThemeColors();

    private:
        bool m_BlockEvents = true;
    };
}