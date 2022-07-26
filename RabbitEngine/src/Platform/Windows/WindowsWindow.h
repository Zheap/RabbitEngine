#pragma once

#include "Rabbit/Core/Window.h"
#include "Rabbit/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Rabbit {

    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const WindowProps& props);

        virtual ~WindowsWindow();

        void OnUpdate() override;

        uint32_t GetWidth() const override { return m_Data.Width; }
        uint32_t GetHeight() const override { return m_Data.Height; }

        void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
        void SetVSync(bool enabled) override;
        bool isVSync() const override;

        virtual void* GetNativeWindow() const override { return m_Window; }

    private:
        virtual void Init(const WindowProps& props);
        virtual void ShutDown();

    private:
        GLFWwindow* m_Window;
        Scope<GraphicsContext> m_Context;

        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool vSync;

            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };
}