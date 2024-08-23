#include "usgpch.h"
#include "Renderer.h"
#include "platform/OpenGL/OpenGLShader.h"

namespace Usagi {
	
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::Init() {
		RenderCommand::Init();
	}
	
	void Renderer::BeginScene(OrthographicCamera& camera) {
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::BeginScene(ProjectionCamera& camera) {
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene() {}

	void Renderer::Submit(
		const Ref<Shader>& shader, 
		const Ref<VertexArray>& vertexArray,
		const glm::mat4& transform
	) {
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}