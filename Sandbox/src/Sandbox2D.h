#pragma once

#include "Rabbit.h"

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

    glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f};

};