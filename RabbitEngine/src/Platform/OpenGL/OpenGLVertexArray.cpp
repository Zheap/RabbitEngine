#include "rbpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Rabbit {

    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
            case Rabbit::ShaderDataType::None:      return GL_FLOAT;
            case Rabbit::ShaderDataType::Float:     return GL_FLOAT;
            case Rabbit::ShaderDataType::Float2:    return GL_FLOAT;
            case Rabbit::ShaderDataType::Float3:    return GL_FLOAT;
            case Rabbit::ShaderDataType::Float4:    return GL_FLOAT;
            case Rabbit::ShaderDataType::Mat3:      return GL_FLOAT;
            case Rabbit::ShaderDataType::Mat4:      return GL_FLOAT;
            case Rabbit::ShaderDataType::Int:       return GL_INT;
            case Rabbit::ShaderDataType::Int2:      return GL_INT;
            case Rabbit::ShaderDataType::Int3:      return GL_INT;
            case Rabbit::ShaderDataType::Int4:      return GL_INT;
            case Rabbit::ShaderDataType::Bool:      return GL_BOOL;
        }
    }

    OpenGLVertexArray::OpenGLVertexArray()
    {
        glCreateVertexArrays(1, &m_RendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_RendererID);
    }

    void OpenGLVertexArray::Bind() const
    {
        glBindVertexArray(m_RendererID);
    }

    void OpenGLVertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
    {
        RB_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

        glBindVertexArray(m_RendererID);
        vertexBuffer->Bind();

        const auto& layout = vertexBuffer->GetLayout();
        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(m_VertexBufferIndex);
            glVertexAttribPointer(m_VertexBufferIndex,
                element.GetComponentCount(),
                ShaderDataTypeToOpenGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                (const void*)(intptr_t)element.Offset);
            m_VertexBufferIndex++;
        }

        m_VertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
    {
        glBindVertexArray(m_RendererID);
        indexBuffer->Bind();

        m_IndexBuffer = indexBuffer;
    }

}