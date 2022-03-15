#pragma once

#include "Rabbit/Window.h"
#include "Rabbit/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Rabbit {

    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const WindowProps& props);

        virtual ~WindowsWindow();

        void OnUpdate() override;

        inline unsigned int GetWidth() const override { return m_Data.Width; }
        inline unsigned int GetHeight() const override { return m_Data.Height; }

        inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
        void SetVSync(bool enabled) override;
        bool isVSync() const override;

        inline virtual void* GetNativeWindow() const override { return m_Window; }

    private:
        virtual void Init(const WindowProps& props);
        virtual void ShutDown();

    private:
        GLFWwindow* m_Window;
        GraphicsContext* m_Context;

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