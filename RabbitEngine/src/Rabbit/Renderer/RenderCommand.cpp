#include "rbpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Rabbit {

    RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}