#include "usgpch.h"
#include "OpenGLVertexArray.h"
#include <glad/glad.h>

namespace Usagi {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) 
	{
		switch (type)
		{
		case Usagi::ShaderDataType::Float:   return GL_FLOAT;
		case Usagi::ShaderDataType::Float2:  return GL_FLOAT;
		case Usagi::ShaderDataType::Float3:  return GL_FLOAT;
		case Usagi::ShaderDataType::Float4:  return GL_FLOAT;
		case Usagi::ShaderDataType::Mat3:	 return GL_FLOAT;
		case Usagi::ShaderDataType::Mat4:	 return GL_FLOAT;
		case Usagi::ShaderDataType::Int:     return GL_INT;
		case Usagi::ShaderDataType::Int2:    return GL_INT;
		case Usagi::ShaderDataType::Int3:    return GL_INT;
		case Usagi::ShaderDataType::Int4:    return GL_INT;
		case Usagi::ShaderDataType::Bool:    return GL_BOOL;
		}
		USG_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray() 
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const 
	{
		glBindVertexArray(m_RendererID);
	};
	void OpenGLVertexArray::Unbind() const 
	{
		glBindVertexArray(0);
	};

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) 
	{
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		USG_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout! ")

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout) {

			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	};
	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& IndexBuffer) {
		glBindVertexArray(m_RendererID);
		IndexBuffer->Bind();

		m_IndexBuffer = IndexBuffer;
	};
}