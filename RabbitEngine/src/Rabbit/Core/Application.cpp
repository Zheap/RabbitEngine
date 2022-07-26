#include "rbpch.h"
#include "Application.h"
#include "Rabbit/Core/Log.h"
#include "Input.h"
#include "Rabbit/Renderer/Renderer.h"
#include <GLFW/glfw3.h>

namespace Rabbit
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application* Application::s_Instance = nullptr;

    Application::Application(const std::string& name)
    {
        RB_PROFILE_FUNCTION();

        RB_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = Scope<Window>(Window::Create(WindowProps(name)));
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
        m_Window->SetVSync(false);

        Renderer::Init();

        m_ImGuiLayer = new ImGuiLayer();
        PushOverLay(m_ImGuiLayer);
    }

    Application::~Application()
    {
        RB_PROFILE_FUNCTION();

        Renderer::Shutdown();
    }

    void Application::Run()
    {
        RB_PROFILE_FUNCTION();

        while (m_Running)
        {
            RB_PROFILE_SCOPE("RunLoop");

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
                {
                    RB_PROFILE_SCOPE("LayerStack OnUpdate");

                    for (Layer* layer : m_LayerStack)
                        layer->OnUpdate(timestep);
                }

                m_ImGuiLayer->Begin();
                {
                    RB_PROFILE_SCOPE("LayerStack OnImGuiRender");

                    for (Layer* layer : m_LayerStack)
                        layer->OnImGuiRender();
                }
                m_ImGuiLayer->End();
            }

            m_Window->OnUpdate();
        }
    }

    void Application::OnEvent(Event& e)
    {
        RB_PROFILE_FUNCTION();

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
        RB_PROFILE_FUNCTION();

        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverLay(Layer* layer)
    {
        RB_PROFILE_FUNCTION();

        m_LayerStack.PushOverLay(layer);
        layer->OnAttach();
    }

    void Application::Close()
    {
        m_Running = false;
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        RB_PROFILE_FUNCTION();

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