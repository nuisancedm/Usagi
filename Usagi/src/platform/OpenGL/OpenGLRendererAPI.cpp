#include "usgpch.h"
#include "OpenGLRendererAPI.h"
#include <glad/glad.h>

namespace Usagi {

	void OpenGLRendererAPI::Init() {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glFrontFace(GL_CCW);

		auto& caps = RendererAPI::GetCapabilities();

		caps.Vendor = (const char*)glGetString(GL_VENDOR);
		caps.Renderer = (const char*)glGetString(GL_RENDERER);
		caps.Version = (const char*)glGetString(GL_VERSION);

		glGetIntegerv(GL_MAX_SAMPLES, &caps.MaxSamples);
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &caps.MaxAnisotropy);

		// --- device info ---
		USG_CORE_INFO("OpenGL Info:");
		USG_CORE_INFO("  Vendor: {0}", caps.Vendor);
		USG_CORE_INFO("  Renderer: {0}", caps.Renderer);
		USG_CORE_INFO("  Version: {0}", caps.Version);
		USG_CORE_INFO("  Max Samples: {0}", caps.MaxSamples);
		USG_CORE_INFO("  Max Texture Max Anisotropy: {0}", caps.MaxAnisotropy);
	};

	
	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray) {
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::SetDepthMask(bool flag) {
		if (flag) {
			glDepthMask(GL_TRUE);
		}
		else {
			glDepthMask(GL_FALSE);
		}
	}
}