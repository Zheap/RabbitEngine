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
        if (Rabbit::Input::IsKeyPressed(RB_KEY_TAB))
            RB_TRACE("Tab Key is pressed!(poll)");
    }

    void OnEvent(Rabbit::Event& event) override
    {
        if (event.GetEventType() == Rabbit::EventType::KeyPressed)
        {
            Rabbit::KeyPressedEvent& e = (Rabbit::KeyPressedEvent&)event;
            if (e.GetKeyCode() == RB_KEY_TAB)
                RB_TRACE("TAB Key is pressed!(event)");
            RB_TRACE("{0}", (char)e.GetKeyCode());
        }
    }
};

class Sandbox : public Rabbit::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());
        PushOverLay(new Rabbit::ImGuiLayer());
    }
    ~Sandbox() {}
};

Rabbit::Application* Rabbit::CreateApplication()
{
    return new Sandbox();
}