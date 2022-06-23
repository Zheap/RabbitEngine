#include "rbpch.h"
#include "Application.h"
#include "Rabbit/Core/Log.h"
#include "Input.h"
#include "Rabbit/Renderer/Renderer.h"
#include <glfw/glfw3.h>

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
        m_Window->SetVSync(false);

        Renderer::Init();

        m_ImGuiLayer = new ImGuiLayer();
        PushOverLay(m_ImGuiLayer);
    }

    void Application::Run()
    {
        while (m_Running)
        {
            float time = (float)glfwGetTime();
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            /*
            * ����layer->OnUpdate()����Ҫ����m_window->OnUpdate()֮ǰ����Ϊm_window->OnUpdate()�����ִ��glfwSwapBuffers(),
            * �ѵ�ǰrender buffer�ڴ��е����ݿ�������Ļ�ϣ����layer->OnUpdate()����m_window->OnUpdate()����Ļ�����Ȼ���layer�������д�뵽buffer�ڴ��У�
            * ��������һ�ν��뵽ѭ��ʱ���ᾭ��glClear(GL_COLOR_BUFFER_BIT)������յ�buffer����ʱ�ֻ�glfwSwapBuffers(), ������Ļ���ڴ��������ǿյģ���ʵ�Ǵ���ClearColor��
            * �����Ļ�����־���layer��Ļ��Ʋ���ʾ
            */
            if (!m_Minimized)
            {
                for (Layer* layer : m_LayerStack)
                    layer->OnUpdate(timestep);

                m_ImGuiLayer->Begin();
                for (Layer* layer : m_LayerStack)
                    layer->OnImGuiRender();
                m_ImGuiLayer->End();
            }

            m_Window->OnUpdate();
        }
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

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
    }

    void Application::PushOverLay(Layer* layer)
    {
        m_LayerStack.PushOverLay(layer);
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        if (e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;
        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

        return false;
    }

}