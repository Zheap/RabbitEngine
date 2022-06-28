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
    {
        RB_PROFILE_SCOPE("Renderer Prep");
        Rabbit::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        Rabbit::RenderCommand::Clear();
    }

    {
        RB_PROFILE_SCOPE("Renderer Draw");
        Rabbit::Renderer2D::BeginScene(m_CameraController.GetCamera());

        Rabbit::Renderer2D::DrawRotationQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(-45.0f), { 0.8f, 0.2f, 0.3f, 1.0f });
        Rabbit::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
        Rabbit::Renderer2D::DrawQuad({ 0.0f,  0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckedboardTexture, 10.0f);

        Rabbit::Renderer2D::EndScene();
    }
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
