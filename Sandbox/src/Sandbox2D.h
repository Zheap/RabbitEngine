#pragma once

#include "Rabbit.h"
#include "ParticleSystem.h"

class Sandbox2D : public Rabbit::Layer
{
public:
    Sandbox2D();
    ~Sandbox2D() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    virtual void OnUpdate(Rabbit::Timestep ts) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Rabbit::Event& e) override;

private:
    Rabbit::OrthographicCameraController m_CameraController;

    Rabbit::Ref<Rabbit::VertexArray> m_SquareVA;
    Rabbit::Ref<Rabbit::Shader> m_FlatColorShader;

    Rabbit::Ref<Rabbit::Texture2D> m_CheckedboardTexture;
    Rabbit::Ref<Rabbit::Texture2D> m_SpriteSheet;
    Rabbit::Ref<Rabbit::SubTexture2D> m_TextureStairs, m_TextureBarrel, m_TextureTree;

    Rabbit::Ref<Rabbit::Framebuffer> m_Framebuffer;

    glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

    ParticleProps m_Particle;
    ParticleSystem m_ParticleSystem;

    uint32_t m_MapWidth, m_MapHeight;
    std::unordered_map<char, Rabbit::Ref<Rabbit::SubTexture2D>> m_TextureMap;
};