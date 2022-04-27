#pragma once

#include "Core.h"

#include "Window.h"
#include "Rabbit/LayerStack.h"
#include "Rabbit/Events/Event.h"
#include "Rabbit/Events/ApplicationEvent.h"
#include "Rabbit/ImGui/ImGuiLayer.h"
#include "Rabbit/Renderer/Shader.h"
#include "Rabbit/Renderer/Buffer.h"
#include "Rabbit/Renderer/VertexArray.h"

namespace Rabbit {

    class RABBIT_API Application
    {
    public:
        Application();
        virtual ~Application();
        
        void Run();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverLay(Layer* layer);

        inline Window& GetWindow() { return *m_Window; }

        inline static Application& Get() { return *s_Instance; }

    private:
        bool OnWindowClose(WindowCloseEvent& e);

        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;
        LayerStack m_LayerStack;

        std::shared_ptr<Shader> m_Shader;
        std::shared_ptr<VertexArray> m_VertexArray;

        std::shared_ptr<Shader> m_BlueShader;
        std::shared_ptr<VertexArray> m_SquareVA;

    private:
        static Application* s_Instance;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}

