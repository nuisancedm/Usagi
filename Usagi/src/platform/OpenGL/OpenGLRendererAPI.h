#pragma once

#include "Usagi/Renderer/RendererAPI.h"
namespace Usagi {
	class OpenGLRendererAPI : public RendererAPI {
	public:
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
	private:
	};
}