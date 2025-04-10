#include "usgpch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "platform/OpenGL/OpenGLVertexArray.h"

namespace Usagi {
	Ref<VertexArray> VertexArray::create() {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::NONE:   USG_CORE_ASSERT(false, "RendererAPI::NONE is not supported yet."); return nullptr;
		case RendererAPI::API::OPENGL: return std::make_shared<OpenGLVertexArray>();
		}

		USG_CORE_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	}
}