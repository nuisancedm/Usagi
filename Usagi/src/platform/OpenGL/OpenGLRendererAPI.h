#pragma once

#include "Usagi/Renderer/RendererAPI.h"
namespace Usagi {
	class OpenGLRendererAPI : public RendererAPI {
	public:
		virtual void Init() override;
		
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, bool enableDepthTesting) override;
		virtual void SetDepthMask(bool flag) override;
		virtual void SetDepthFn(DepthFnType type) override;
	private:
	};
}