#include "Sandbox2D.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
{
}

void Sandbox2D::OnAttach()
{
    RB_PROFILE_FUNCTION();

    m_CheckedboardTexture = Rabbit::Texture2D::Create("assets/textures/Checkerboard.png");
    m_SpriteSheet = Rabbit::Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");

    m_TextureStairs = Rabbit::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7, 6 }, { 128, 128 });
    m_TextureBarrel = Rabbit::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 8, 2 }, { 128, 128 });
    m_TextureTree = Rabbit::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2, 1 }, { 128, 128 }, { 1, 2 });


    // Init here
    m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
    m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
    m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
    m_Particle.LifeTime = 1.0f;
    m_Particle.Velocity = { 0.0f, 0.0f };
    m_Particle.VelocityVariation = { 3.0f, 1.0f };
    m_Particle.Position = { 0.0f, 0.0f };
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

#if 0
    {
        RB_PROFILE_SCOPE("Renderer Draw");

        static float rotation = 0.0f;
        rotation += ts * 50.0f;

        Rabbit::Renderer2D::BeginScene(m_CameraController.GetCamera());

        Rabbit::Renderer2D::DrawRotationQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(-45.0f), { 0.8f, 0.2f, 0.3f, 1.0f });
        Rabbit::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
        Rabbit::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_SquareColor);
        Rabbit::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckedboardTexture, 10.0f);
        Rabbit::Renderer2D::DrawRotationQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), m_CheckedboardTexture, 20.0f);

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
#endif

    if (Rabbit::Input::IsMouseButtonPressed(RB_MOUSE_BUTTON_LEFT))
    {
        auto [x, y] = Rabbit::Input::GetMousePosition();
        auto width = Rabbit::Application::Get().GetWindow().GetWidth();
        auto height = Rabbit::Application::Get().GetWindow().GetHeight();

        auto bounds = m_CameraController.GetBounds();
        auto pos = m_CameraController.GetCamera().GetPosition();
        x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
        y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
        m_Particle.Position = { x + pos.x, y + pos.y };
        for (int i = 0; i < 5; i++)
            m_ParticleSystem.Emit(m_Particle);
    }

    m_ParticleSystem.OnUpdate(ts);
    m_ParticleSystem.OnRender(m_CameraController.GetCamera());

    Rabbit::Renderer2D::BeginScene(m_CameraController.GetCamera());
    Rabbit::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.5f }, { 1.0f, 1.0f }, m_TextureStairs);
    Rabbit::Renderer2D::DrawQuad({ 1.0f, 0.0f, 0.5f }, { 1.0f, 1.0f }, m_TextureBarrel);
    Rabbit::Renderer2D::DrawQuad({ -1.0f, 0.0f, 0.5f }, { 1.0f, 2.0f }, m_TextureTree);
    Rabbit::Renderer2D::EndScene();
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
