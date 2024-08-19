#include "usgpch.h"
#include "Shader.h"
#include "Renderer.h"
#include "platform/OpenGL/OpenGLShader.h"

namespace Usagi {
	Shader* Shader::create(const std::string& vertexSrc, const std::string& fragmentSrc) {
		switch (Renderer::GetAPI()) 
		{
		case RendererAPI::API::NONE:   USG_CORE_ASSERT(false, "RendererAPI::NONE is not supported yet."); return nullptr;
		case RendererAPI::API::OPENGL: return new OpenGLShader(vertexSrc, fragmentSrc);
		}

		USG_CORE_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	}
}



