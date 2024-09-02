#pragma once
#include "RendererAPI.h"

namespace Usagi {
	class RenderCommand {
	public:
		inline static void Init() {
			s_RendererAPI->Init();
		}

		inline static void SetClearColor(const glm::vec4& color) {
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear() {
			s_RendererAPI->Clear();
		}
		
		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, bool enableDepthTesting) {
			s_RendererAPI->DrawIndexed(vertexArray, enableDepthTesting);
		}

		inline static void SetDepthMask(bool flag) {
			s_RendererAPI->SetDepthMask(flag);
		}

		inline static void SetDepthFn(RendererAPI::DepthFnType type) {
			s_RendererAPI->SetDepthFn(type);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};
}