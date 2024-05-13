#include "usgpch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "platform/OpenGL/OpenGLBuffer.h"

namespace Usagi {
	
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) 
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::NONE: 
			USG_CORE_ASSERT(false, "RendererAPI::NONE is not supported yet.");
			return nullptr;
		case RendererAPI::OPENGL: return new OpenGLVertexBuffer(vertices, size);
		}

		USG_CORE_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size) 
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::NONE:
			USG_CORE_ASSERT(false, "RendererAPI::NONE is not supported yet.");
			return nullptr;
		case RendererAPI::OPENGL:
			return new OpenGLIndexBuffer(indices, size);
		}

		USG_CORE_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	}
}