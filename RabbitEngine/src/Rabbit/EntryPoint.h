#pragma once

#ifdef RB_PLATFORM_WINDOWS

extern Rabbit::Application* Rabbit::CreateApplication();

int main(int argc, char** argv)
{
    Rabbit::Log::Init();
    RB_CORE_WARN("Initialized log!");
    int a = 5;
    RB_INFO("Hello! Var = {0}", a);

    auto app = Rabbit::CreateApplication();
    app->Run();
    delete app;
}

#endif
