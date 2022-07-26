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
            * 这里layer->OnUpdate()必须要放在m_window->OnUpdate()之前，因为m_window->OnUpdate()里面会执行glfwSwapBuffers(),
            * 把当前render buffer内存中的数据拷贝到屏幕上；如果layer->OnUpdate()放在m_window->OnUpdate()后面的话，虽然会把layer层的数据写入到buffer内存中，
            * 但是在下一次进入到循环时，会经历glClear(GL_COLOR_BUFFER_BIT)，会清空掉buffer，此时又会glfwSwapBuffers(), 给到屏幕的内存数据又是空的（其实是带有ClearColor）
            * 因此屏幕上体现就是layer层的绘制不显示
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