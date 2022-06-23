#pragma once

#ifdef RB_PLATFORM_WINDOWS

extern Rabbit::Application* Rabbit::CreateApplication();

int main(int argc, char** argv)
{
    Rabbit::Log::Init();

    RB_PROFILE_BEGIN_SESSION("Startup", "RabbitProfile-Startup.json");
    auto app = Rabbit::CreateApplication();
    RB_PROFILE_END_SESSION();

    RB_PROFILE_BEGIN_SESSION("Runtime", "RabbitProfile-Runtime.json");
    app->Run();
    RB_PROFILE_END_SESSION();

    RB_PROFILE_BEGIN_SESSION("ShutDown", "RabbitProfile-ShutDown.json");
    delete app;
    RB_PROFILE_END_SESSION();
}

#endif
