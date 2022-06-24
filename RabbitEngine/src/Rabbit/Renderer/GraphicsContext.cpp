#include "rbpch.h"
#include "GraphicsContext.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Rabbit {

    Scope<Rabbit::GraphicsContext> GraphicsContext::Create(void* window)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            RB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
        }

        RB_CORE_ASSERT(false, "Unknow RendererAPI");
        return nullptr;
    }

}