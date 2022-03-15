#include "rbpch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>  
#include <GLFW/glfw3.h> 

namespace Rabbit {
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        : m_WindowHandle(windowHandle)
    {
        RB_CORE_ASSERT(m_WindowHandle, "Window handle is null!");
    }
    void OpenGLContext::Init()
    {
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        RB_CORE_ASSERT(status, "Failed to initialize Glad!");
    }
    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }
}