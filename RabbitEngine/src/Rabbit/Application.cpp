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
            * ����layer->OnUpdate()����Ҫ����m_window->OnUpdate()֮ǰ����Ϊm_window->OnUpdate()�����ִ��glfwSwapBuffers(),
            * �ѵ�ǰrender buffer�ڴ��е����ݿ�������Ļ�ϣ����layer->OnUpdate()����m_window->OnUpdate()����Ļ�����Ȼ���layer�������д�뵽buffer�ڴ��У�
            * ��������һ�ν��뵽ѭ��ʱ���ᾭ��glClear(GL_COLOR_BUFFER_BIT)������յ�buffer����ʱ�ֻ�glfwSwapBuffers(), ������Ļ���ڴ��������ǿյģ���ʵ�Ǵ���ClearColor��
            * �����Ļ�����־���layer��Ļ��Ʋ���ʾ
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