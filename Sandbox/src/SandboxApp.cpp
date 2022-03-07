#include <Rabbit.h>

class ExampleLayer : public Rabbit::Layer
{
public:
    ExampleLayer()
        : Layer("Example")
    {
    }

    void OnUpdate() override
    {
        RB_INFO("ExampleLayer::Update");
    }

    void OnEvent(Rabbit::Event& event) override
    {
        RB_TRACE("{0}", event);
    }
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