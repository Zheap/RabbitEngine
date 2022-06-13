#include <Rabbit.h>
#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class ExampleLayer : public Rabbit::Layer
{
public:
    ExampleLayer()
        : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_SquarePosition(0.0f)
    {
        m_VertexArray.reset(Rabbit::VertexArray::Create());

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };

        // VertexBuffer
        Rabbit::Ref<Rabbit::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(Rabbit::VertexBuffer::Create(vertices, sizeof(vertices)));
        Rabbit::BufferLayout layout = {
            { Rabbit::ShaderDataType::Float3, "a_Position" },
            { Rabbit::ShaderDataType::Float4, "a_Color"    }
        };
        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        // IndexBuffer
        uint32_t indices[3] = { 0, 1, 2 };
        Rabbit::Ref<Rabbit::IndexBuffer> indexBuffer;
        indexBuffer.reset(Rabbit::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(indexBuffer);


        float squareVertices[5 * 4] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
        };
        m_SquareVA.reset(Rabbit::VertexArray::Create());
        Rabbit::Ref<Rabbit::VertexBuffer> squareVB;
        squareVB.reset(Rabbit::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

        squareVB->SetLayout({
            { Rabbit::ShaderDataType::Float3, "a_Position" },
            { Rabbit::ShaderDataType::Float2, "a_TexCoord" }
            });
        m_SquareVA->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        Rabbit::Ref<Rabbit::IndexBuffer> squareIB;
        squareIB.reset(Rabbit::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        m_SquareVA->SetIndexBuffer(squareIB);


        std::string vertexSrc = R"(
            #version 330 core
            
            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec3 v_Position;
            out vec4 v_Color;

            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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

        m_Shader = Rabbit::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

        std::string FlatColorShaderVertexSrc = R"(
            #version 330 core
            
            layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec3 v_Position;

            void main()
            {
                v_Position = a_Position;
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
            }
        )";

        std::string FlatColorShaderFragmentSrc = R"(
            #version 330 core
            
            layout(location = 0) out vec4 color;

            in vec3 v_Position;

            uniform vec3 u_Color;

            void main()
            {
                color = vec4(u_Color, 1.0f);
            }
        )";

        m_FlatColorShader = Rabbit::Shader::Create("FlatColor", FlatColorShaderVertexSrc, FlatColorShaderFragmentSrc);

        auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

        // 我理解texture的本质是：把纹理(图片)的数据缓存到GPU中，配合着texCoord，将数据送给Fragment Shader里的color成员，绘制color即绘制纹理(图片)
        m_Texture = Rabbit::Texture2D::Create("assets/textures/Checkerboard.png");
        m_ChernoLogoTexture = Rabbit::Texture2D::Create("assets/textures/ChernoLogo.png");

        std::dynamic_pointer_cast<Rabbit::OpenGLShader>(textureShader)->Bind();
        std::dynamic_pointer_cast<Rabbit::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);  // texture slot
    }

    void OnUpdate(Rabbit::Timestep ts) override
    {
        RB_TRACE("Delta time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

        if (Rabbit::Input::IsKeyPressed(RB_KEY_LEFT))
            m_CameraPosition.x -= m_CameraMoveSpeed * ts;
        else if (Rabbit::Input::IsKeyPressed(RB_KEY_RIGHT))
            m_CameraPosition.x += m_CameraMoveSpeed * ts;

        if (Rabbit::Input::IsKeyPressed(RB_KEY_UP))
            m_CameraPosition.y += m_CameraMoveSpeed * ts;
        else if (Rabbit::Input::IsKeyPressed(RB_KEY_DOWN))
            m_CameraPosition.y -= m_CameraMoveSpeed * ts;

        if (Rabbit::Input::IsKeyPressed(RB_KEY_A))
            m_CameraRotation += m_CameraRotationSpeed * ts;
        if (Rabbit::Input::IsKeyPressed(RB_KEY_D))
            m_CameraRotation -= m_CameraRotationSpeed * ts;

        //if (Rabbit::Input::IsKeyPressed(RB_KEY_J))
        //    m_SquarePosition.x -= m_SquareMoveSpeed * ts;
        //else if (Rabbit::Input::IsKeyPressed(RB_KEY_L))
        //    m_SquarePosition.x += m_SquareMoveSpeed * ts;

        //if (Rabbit::Input::IsKeyPressed(RB_KEY_I))
        //    m_SquarePosition.y += m_SquareMoveSpeed * ts;
        //else if (Rabbit::Input::IsKeyPressed(RB_KEY_K))
        //    m_SquarePosition.y -= m_SquareMoveSpeed * ts;


        Rabbit::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        Rabbit::RenderCommand::Clear();

        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        Rabbit::Renderer::BeginScene(m_Camera);

        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

        std::dynamic_pointer_cast<Rabbit::OpenGLShader>(m_FlatColorShader)->Bind();
        std::dynamic_pointer_cast<Rabbit::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

        for (int y = 0; y < 20; y++)
        {
            for (int x = 0; x < 20; x++)
            {
                glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
                Rabbit::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
            }
        }

        auto textureShader = m_ShaderLibrary.Get("Texture");
        // Fragment里面的uniform sampler2D u_Texture; 会默认在buffer里面对应的slot = 0 位置查找texture data，所以即使上面m_TextureShader没有uploadUniformInt(u_Texture)，图片也能正常绘制
        m_Texture->Bind();
        Rabbit::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

        m_ChernoLogoTexture->Bind();
        Rabbit::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

        // Triangle
        // Rabbit::Renderer::Submit(m_Shader, m_VertexArray);

        Rabbit::Renderer::EndScene();
    }

    void OnImGuiRender() override
    {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
        ImGui::End();
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
    Rabbit::ShaderLibrary m_ShaderLibrary;
    Rabbit::Ref<Rabbit::Shader> m_Shader;
    Rabbit::Ref<Rabbit::VertexArray> m_VertexArray;

    Rabbit::Ref<Rabbit::Shader> m_FlatColorShader;
    Rabbit::Ref<Rabbit::VertexArray> m_SquareVA;

    Rabbit::Ref<Rabbit::Texture> m_Texture, m_ChernoLogoTexture;

    Rabbit::OrthographicCamera m_Camera;
    glm::vec3 m_CameraPosition;
    float m_CameraMoveSpeed = 5.0f;

    float m_CameraRotation = 0.0f;
    float m_CameraRotationSpeed = 180.0f;

    glm::vec3 m_SquarePosition;
    float m_SquareMoveSpeed = 5.0f;

    glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
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