#include "rbpch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Rabbit {


    Rabbit::Ref<Rabbit::VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            RB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return Rabbit::CreateRef<OpenGLVertexBuffer>(vertices, size);
        }

        RB_CORE_ASSERT(false, "Unknow RendererAPI");
        return nullptr;
    }

    Rabbit::Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            RB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return Rabbit::CreateRef<OpenGLIndexBuffer>(indices, count);
        }

        RB_CORE_ASSERT(false, "Unknow RendererAPI");
        return nullptr;
    }

}