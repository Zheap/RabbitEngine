#pragma once

#include "Core.h"

#include "Window.h"
#include "Rabbit/Core/LayerStack.h"
#include "Rabbit/Events/Event.h"
#include "Rabbit/Events/ApplicationEvent.h"
#include "Rabbit/Core/Timestep.h"
#include "Rabbit/ImGui/ImGuiLayer.h"

namespace Rabbit {

    class Application
    {
    public:
        Application();
        virtual ~Application();
        
        void Run();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverLay(Layer* layer);

        Window& GetWindow() { return *m_Window; }

        static Application& Get() { return *s_Instance; }

    private:
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;
        bool m_Minimized = false;
        LayerStack m_LayerStack;
        Timestep m_Timestep;
        float m_LastFrameTime = 0.0f;

    private:
        static Application* s_Instance;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}

