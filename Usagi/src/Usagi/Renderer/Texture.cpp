#include "usgpch.h"
#include "Texture.h"
#include "Renderer.h"
#include "platform/OpenGL/OpenGLTexture.h"

namespace Usagi {
	Ref<Texture2D> Texture2D::Create(const std::string& filepath) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::NONE:   USG_CORE_ASSERT(false, "RendererAPI::NONE is not supported yet."); return nullptr;
		case RendererAPI::API::OPENGL: return  std::make_shared<OpenGLTexture2D>(filepath);
		}

		USG_CORE_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	}
}