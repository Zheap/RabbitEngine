#include "rbpch.h"
#include "RendererAPI.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Rabbit {

    RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

    Scope<Rabbit::RendererAPI> RendererAPI::Create()
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            RB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateScope<OpenGLRendererAPI>();
        }

        RB_CORE_ASSERT(false, "Unknow RendererAPI");
        return nullptr;
    }


}