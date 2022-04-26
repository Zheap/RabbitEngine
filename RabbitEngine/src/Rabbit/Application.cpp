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
            * ����layer->OnUpdate()����Ҫ����m_window->OnUpdate()֮ǰ����Ϊm_window->OnUpdate()�����ִ��glfwSwapBuffers(),
            * �ѵ�ǰrender buffer�ڴ��е����ݿ�������Ļ�ϣ����layer->OnUpdate()����m_window->OnUpdate()����Ļ�����Ȼ���layer�������д�뵽buffer�ڴ��У�
            * ��������һ�ν��뵽ѭ��ʱ���ᾭ��glClear(GL_COLOR_BUFFER_BIT)������յ�buffer����ʱ�ֻ�glfwSwapBuffers(), ������Ļ���ڴ��������ǿյģ���ʵ�Ǵ���ClearColor��
            * �����Ļ�����־���layer��Ļ��Ʋ���ʾ
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