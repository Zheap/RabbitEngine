#include "Sandbox2D.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
    m_SquareVA = Rabbit::VertexArray::Create();

    float squareVertices[3 * 4] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
    };
    Rabbit::Ref<Rabbit::VertexBuffer> squareVB;
    squareVB = Rabbit::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

    squareVB->SetLayout({
        { Rabbit::ShaderDataType::Float3, "a_Position" },
        });
    m_SquareVA->AddVertexBuffer(squareVB);

    uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
    Rabbit::Ref<Rabbit::IndexBuffer> squareIB;
    squareIB = Rabbit::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
    m_SquareVA->SetIndexBuffer(squareIB);

    m_FlatColorShader = Rabbit::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Rabbit::Timestep ts)
{
    // OnUpdate
    m_CameraController.OnUpdate(ts);

    // Render
    Rabbit::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
    Rabbit::RenderCommand::Clear();

    Rabbit::Renderer::BeginScene(m_CameraController.GetCamera());

    std::dynamic_pointer_cast<Rabbit::OpenGLShader>(m_FlatColorShader)->Bind();
    std::dynamic_pointer_cast<Rabbit::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

    Rabbit::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

    Rabbit::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(Rabbit::Event& e)
{
    m_CameraController.OnEvent(e);
}
