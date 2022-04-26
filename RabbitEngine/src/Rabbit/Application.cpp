#include "rbpch.h"
#include "Application.h"
#include "Rabbit/Log.h"
#include "Input.h"

#include <glad/glad.h>

namespace Rabbit
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application* Application::s_Instance = nullptr;

    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
            case Rabbit::ShaderDataType::None:      return GL_FLOAT;
            case Rabbit::ShaderDataType::Float:     return GL_FLOAT;
            case Rabbit::ShaderDataType::Float2:    return GL_FLOAT;
            case Rabbit::ShaderDataType::Float3:    return GL_FLOAT;
            case Rabbit::ShaderDataType::Float4:    return GL_FLOAT;
            case Rabbit::ShaderDataType::Mat3:      return GL_FLOAT;
            case Rabbit::ShaderDataType::Mat4:      return GL_FLOAT;
            case Rabbit::ShaderDataType::Int:       return GL_INT;
            case Rabbit::ShaderDataType::Int2:      return GL_INT;
            case Rabbit::ShaderDataType::Int3:      return GL_INT;
            case Rabbit::ShaderDataType::Int4:      return GL_INT;
            case Rabbit::ShaderDataType::Bool:      return GL_BOOL;
        }
    }

    Application::Application()
    {
        RB_CORE_ASSERT(!s_Instance, "Application already exists!")
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverLay(m_ImGuiLayer);

        // VertexArray
        glGenVertexArrays(1, &m_VertexArray);
        glBindVertexArray(m_VertexArray);

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };

        // VertexBuffer
        m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

        {
            BufferLayout layout = {   
                { ShaderDataType::Float3, "a_Position" },
                { ShaderDataType::Float4, "a_Color"    }
            };

            m_VertexBuffer->SetLayout(layout);
        }

        uint32_t index = 0;
        const auto& layout = m_VertexBuffer->GetLayout();
        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, 
                element.GetComponentCount(), 
                ShaderDataTypeToOpenGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE, 
                layout.GetStride(), 
                (const void*)element.Offset);
            index++;
        }

        // IndexBuffer
        uint32_t indices[3] = { 0, 1, 2 };
        m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

        std::string vertexSrc = R"(
            #version 330 core
            
            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            out vec3 v_Position;
            out vec4 v_Color;

            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = vec4(a_Position, 1.0);
            }
        )";

        std::string fragmentSrc = R"(
            #version 330 core
            
            layout(location = 0) out vec4 color;

            in vec3 v_Position;
            in vec4 v_Color;

            void main()
            {
                color = vec4(v_Position * 0.5 + 0.5, 1.0);
                color = v_Color;
            }
        )";

        m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        while (m_Running)
        {
            glClearColor(0.1, 0.1, 0.1, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            m_Shader->Bind();
            glBindVertexArray(m_VertexArray);
            glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

            /*
            * 这里layer->OnUpdate()必须要放在m_window->OnUpdate()之前，因为m_window->OnUpdate()里面会执行glfwSwapBuffers(),
            * 把当前render buffer内存中的数据拷贝到屏幕上；如果layer->OnUpdate()放在m_window->OnUpdate()后面的话，虽然会把layer层的数据写入到buffer内存中，
            * 但是在下一次进入到循环时，会经历glClear(GL_COLOR_BUFFER_BIT)，会清空掉buffer，此时又会glfwSwapBuffers(), 给到屏幕的内存数据又是空的（其实是带有ClearColor）
            * 因此屏幕上体现就是layer层的绘制不显示
            */
            for (Layer* layer : m_LayerStack)
                layer->OnUpdate();

            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack)
                layer->OnImGuiRender();
            m_ImGuiLayer->End();

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