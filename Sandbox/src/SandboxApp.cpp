#include <Rabbit.h>

#include "imgui/imgui.h"
class ExampleLayer : public Rabbit::Layer
{
public:
    ExampleLayer()
        : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
    {
        m_VertexArray.reset(Rabbit::VertexArray::Create());

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };

        // VertexBuffer
        std::shared_ptr<Rabbit::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(Rabbit::VertexBuffer::Create(vertices, sizeof(vertices)));
        Rabbit::BufferLayout layout = {
            { Rabbit::ShaderDataType::Float3, "a_Position" },
            { Rabbit::ShaderDataType::Float4, "a_Color"    }
        };
        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        // IndexBuffer
        uint32_t indices[3] = { 0, 1, 2 };
        std::shared_ptr<Rabbit::IndexBuffer> indexBuffer;
        indexBuffer.reset(Rabbit::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(indexBuffer);


        float squareVertices[3 * 4] = {
            -0.75f, -0.75f, 0.0f,
             0.75f, -0.75f, 0.0f,
             0.75f,  0.75f, 0.0f,
            -0.75f,  0.75f, 0.0f
        };
        m_SquareVA.reset(Rabbit::VertexArray::Create());
        std::shared_ptr<Rabbit::VertexBuffer> squareVB;
        squareVB.reset(Rabbit::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

        squareVB->SetLayout({
            { Rabbit::ShaderDataType::Float3, "a_Position" }
            });
        m_SquareVA->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        std::shared_ptr<Rabbit::IndexBuffer> squareIB;
        squareIB.reset(Rabbit::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        m_SquareVA->SetIndexBuffer(squareIB);


        std::string vertexSrc = R"(
            #version 330 core
            
            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;

            out vec3 v_Position;
            out vec4 v_Color;

            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

        m_Shader.reset(new Rabbit::Shader(vertexSrc, fragmentSrc));

        std::string BlueShaderVertexSrc = R"(
            #version 330 core
            
            layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;

            out vec3 v_Position;

            void main()
            {
                v_Position = a_Position;
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
            }
        )";

        std::string BlueShaderFragmentSrc = R"(
            #version 330 core
            
            layout(location = 0) out vec4 color;

            in vec3 v_Position;

            void main()
            {
                color = vec4(0.2, 0.3, 0.8, 1.0);
            }
        )";

        m_BlueShader.reset(new Rabbit::Shader(BlueShaderVertexSrc, BlueShaderFragmentSrc));
    }

    void OnUpdate() override
    {
        if (Rabbit::Input::IsKeyPressed(RB_KEY_LEFT))
            m_CameraPosition.x -= m_CameraMoveSpeed;
        else if (Rabbit::Input::IsKeyPressed(RB_KEY_RIGHT))
            m_CameraPosition.x += m_CameraMoveSpeed;

        if (Rabbit::Input::IsKeyPressed(RB_KEY_DOWN))
            m_CameraPosition.y -= m_CameraMoveSpeed;
        else if (Rabbit::Input::IsKeyPressed(RB_KEY_UP))
            m_CameraPosition.y += m_CameraMoveSpeed;

        if (Rabbit::Input::IsKeyPressed(RB_KEY_A))
            m_CameraRotation += m_CameraRotationSpeed;
        if (Rabbit::Input::IsKeyPressed(RB_KEY_D))
            m_CameraRotation -= m_CameraRotationSpeed;


        Rabbit::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        Rabbit::RenderCommand::Clear();

        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        Rabbit::Renderer::BeginScene(m_Camera);

        Rabbit::Renderer::Submit(m_BlueShader, m_SquareVA);
        Rabbit::Renderer::Submit(m_Shader, m_VertexArray);

        Rabbit::Renderer::EndScene();
    }

    void OnImGuiRender() override
    {

    }

    void OnEvent(Rabbit::Event& event) override
    {
        //Rabbit::EventDispatcher dispatcher(event);
        //dispatcher.Dispatch<Rabbit::KeyPressedEvent>(RB_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
    }

    bool OnKeyPressedEvent(Rabbit::KeyPressedEvent& event)
    {
        // chunky movement on event system

        //if (event.GetKeyCode() == RB_KEY_LEFT)
        //    m_CameraPosition.x -= m_CameraSpeed;

        //if (event.GetKeyCode() == RB_KEY_RIGHT)
        //    m_CameraPosition.x += m_CameraSpeed;

        //if (event.GetKeyCode() == RB_KEY_DOWN)
        //    m_CameraPosition.y -= m_CameraSpeed;

        //if (event.GetKeyCode() == RB_KEY_UP)
        //    m_CameraPosition.y += m_CameraSpeed;

        return false;
    }

private:
    std::shared_ptr<Rabbit::Shader> m_Shader;
    std::shared_ptr<Rabbit::VertexArray> m_VertexArray;

    std::shared_ptr<Rabbit::Shader> m_BlueShader;
    std::shared_ptr<Rabbit::VertexArray> m_SquareVA;

    Rabbit::OrthographicCamera m_Camera;
    glm::vec3 m_CameraPosition;
    float m_CameraMoveSpeed = 0.1f;

    float m_CameraRotation = 0.0f;
    float m_CameraRotationSpeed = 2.0f;
};

class Sandbox : public Rabbit::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());

    }
    ~Sandbox() {}
};

Rabbit::Application* Rabbit::CreateApplication()
{
    return new Sandbox();
}