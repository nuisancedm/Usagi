#pragma once
#include "RenderCommand.h"
#include "usagi/Renderer/Camera.h"
#include "Usagi/Renderer/Shader.h"

namespace Usagi {
	
	class Renderer
	{
	public:

		static void Init();
		static void BeginScene(OrthographicCamera& camera);
		static void BeginScene(ProjectionCamera& camera);
		static void EndScene();

		static void Submit(
			const Ref<Shader>& shader,
			const Ref<VertexArray>& vertexArray,
			const glm::mat4& transform = glm::mat4(1.0f), bool enableDepthTesting = true);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};
}


