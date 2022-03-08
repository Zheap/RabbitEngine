#include "rbpch.h"
#include "Application.h"
#include "Rabbit/Log.h"

#include <glad/glad.h>

namespace Rabbit
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        RB_CORE_ASSERT(!s_Instance, "Application already exists!")
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        while (m_Running)
        {
            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            /*
            * 这里layer->OnUpdate()必须要放在m_window->OnUpdate()之前，因为m_window->OnUpdate()里面会执行glfwSwapBuffers(),
            * 把当前render buffer内存中的数据拷贝到屏幕上；如果layer->OnUpdate()放在m_window->OnUpdate()后面的话，虽然会把layer层的数据写入到buffer内存中，
            * 但是在下一次进入到循环时，会经历glClear(GL_COLOR_BUFFER_BIT)，会清空掉buffer，此时又会glfwSwapBuffers(), 给到屏幕的内存数据又是空的（其实是带有ClearColor）
            * 因此屏幕上体现就是layer层的绘制不显示
            */
            for (Layer* layer : m_LayerStack)
                layer->OnUpdate();

            m_Window->OnUpdate();
        }
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
                break;
        }
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverLay(Layer* layer)
    {
        m_LayerStack.PushOverLay(layer);
        layer->OnAttach();
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }
}