#include "rbpch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>  
#include <GLFW/glfw3.h> 

namespace Rabbit {

    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        : m_WindowHandle(windowHandle)
    {
        RB_PROFILE_FUNCTION();

        RB_CORE_ASSERT(m_WindowHandle, "Window handle is null!");
    }

    void OpenGLContext::Init()
    {
        RB_PROFILE_FUNCTION();

        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        RB_CORE_ASSERT(status, "Failed to initialize Glad!");

        RB_CORE_INFO("OpenGL Info: ");
        RB_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
        RB_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
        RB_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

#ifdef RB_ENABLE_ASSERTS
        int versionMajor;
        int versionMinor;
        glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
        glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

        RB_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Hazel requires at least OpenGL version 4.5!");
#endif
    }

    void OpenGLContext::SwapBuffers()
    {
        RB_PROFILE_FUNCTION();

        glfwSwapBuffers(m_WindowHandle);
    }
}