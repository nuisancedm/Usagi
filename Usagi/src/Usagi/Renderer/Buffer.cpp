#include "usgpch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "platform/OpenGL/OpenGLBuffer.h"

namespace Usagi {
	
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) 
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE: 
			USG_CORE_ASSERT(false, "RendererAPI::NONE is not supported yet.");
			return nullptr;
		case RendererAPI::API::OPENGL: return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}

		USG_CORE_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) 
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:
			USG_CORE_ASSERT(false, "RendererAPI::NONE is not supported yet.");
			return nullptr;
		case RendererAPI::API::OPENGL:
			return   std::make_shared<OpenGLIndexBuffer>(indices, count);
		}

		USG_CORE_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	}
}