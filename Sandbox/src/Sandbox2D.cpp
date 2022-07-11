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
    RB_PROFILE_FUNCTION();

    m_CheckedboardTexture = Rabbit::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
    RB_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Rabbit::Timestep ts)
{
    RB_PROFILE_FUNCTION();

    // OnUpdate
    m_CameraController.OnUpdate(ts);

    // Render
    Rabbit::Renderer2D::ResetStats();
    {
        RB_PROFILE_SCOPE("Renderer Prep");
        Rabbit::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        Rabbit::RenderCommand::Clear();
    }

    {
        RB_PROFILE_SCOPE("Renderer Draw");

        static float rotation = 0.0f;
        rotation += ts * 50.0f;

        Rabbit::Renderer2D::BeginScene(m_CameraController.GetCamera());

        Rabbit::Renderer2D::DrawRotationQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
        Rabbit::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
        Rabbit::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
        Rabbit::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckedboardTexture, 10.0f);
        Rabbit::Renderer2D::DrawRotationQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_CheckedboardTexture, 20.0f);

        Rabbit::Renderer2D::EndScene();

        Rabbit::Renderer2D::BeginScene(m_CameraController.GetCamera());
        for (float y = -5.0f; y < 5.0f; y += 0.5f)
        {
            for (float x = -5.0f; x < 5.0f; x += 0.5f)
            {
                glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
                Rabbit::Renderer2D::DrawQuad({x, y}, { 0.45f, 0.45f }, color);
            }
        }
        Rabbit::Renderer2D::EndScene();

    }
}

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Settings");

    auto stats = Rabbit::Renderer2D::GetStats();
    ImGui::Text("Renderer2D Stats: ");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

    ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(Rabbit::Event& e)
{
    m_CameraController.OnEvent(e);
}
