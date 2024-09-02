#pragma once
#include <Usagi.h>
#include "platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"
#include <glm/glm.hpp>

class simplePBRLayer : public Usagi::Layer {
public:
	simplePBRLayer()
		:m_Camera(glm::perspectiveFov(glm::radians(45.0f), 1280.0f, 720.0f, 0.1f, 10000.0f))
	{
		// quad FOR FRAMEBUFFER
		float quadVertices[5 * 4] = {
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		};
		unsigned int quadIndices[6] = { 0,1,2,2,3,0 };
		m_quadVA = Usagi::VertexArray::create();
		Usagi::Ref<Usagi::VertexBuffer> vertexBuffer = Usagi::VertexBuffer::Create(quadVertices, sizeof(quadVertices));
		Usagi::BufferLayout quadLayout = {
			{Usagi::ShaderDataType::Float3, "a_Position"},
			{Usagi::ShaderDataType::Float2, "a_TexCoord"}
		};
		vertexBuffer->SetLayout(quadLayout);
		Usagi::Ref<Usagi::IndexBuffer> indexBuffer = Usagi::IndexBuffer::Create(quadIndices, 6);
		m_quadVA->AddVertexBuffer(vertexBuffer);
		m_quadVA->SetIndexBuffer(indexBuffer);

		// create framebuffer
		m_Framebuffer = Usagi::Framebuffer::Create(1280, 720, Usagi::FramebufferFormat::RGBA16F);
		m_FinalPresentBuffer = Usagi::Framebuffer::Create(1280, 720, Usagi::FramebufferFormat::RGBA8);

		// load Shaders
		m_ShaderLibrary.Load("assets/shaders/simplePBR.glsl");
		m_ShaderLibrary.Load("assets/shaders/quad.glsl");
		m_ShaderLibrary.Load("assets/shaders/hdr.glsl");
		m_ShaderLibrary.Load("assets/shaders/first3D.glsl");

		// m_CubeTexture_Pinetree = Usagi::TextureCube::Create("assets/textures/skybox_pinetree/Arches_E_PineTree_Radiance.tga");
		m_EnvironmentCubeMap = Usagi::TextureCube::Create("assets/textures/skybox_pinetree/Arches_E_PineTree_Radiance.tga");
		m_EnvironmentIrradiance = Usagi::TextureCube::Create("assets/textures/skybox_pinetree/Arches_E_PineTree_Irradiance.tga");
		m_AlbedoInput.TextureMap = Usagi::Texture2D::Create("assets/textures/cerberus/cerberus_A.png");
		m_MetalnessInput.TextureMap = Usagi::Texture2D::Create("assets/textures/cerberus/cerberus_M.png");
		m_RoughnessInput.TextureMap = Usagi::Texture2D::Create("assets/textures/cerberus/cerberus_R.png");
		m_NormalInput.TextureMap = Usagi::Texture2D::Create("assets/textures/cerberus/cerberus_N.png");
		m_BRDFLUT = Usagi::Texture2D::Create("assets/textures/BRDF_LUT.tga");
		std::vector<std::string> EnvironmentSkyPaths = {
			"assets/textures/skybox/right.jpg",
			"assets/textures/skybox/left.jpg",
			"assets/textures/skybox/top.jpg",
			"assets/textures/skybox/bottom.jpg",
			"assets/textures/skybox/front.jpg",
			"assets/textures/skybox/back.jpg"
		};
		m_EnvironmentSky = Usagi::TextureCube::Create(EnvironmentSkyPaths);

		// Load Meshs
		m_SphereMesh.reset(new Usagi::Mesh("assets/models/Sphere.fbx"));
		m_CerberusMesh.reset(new Usagi::Mesh("assets/models/Cerberus.fbx"));
		m_CubeMesh.reset(new Usagi::Mesh("assets/models/Cube.fbx"));

		m_Light.Direction = { -0.5f, -0.5f, 1.0f };
		m_Light.Radiance = { 1.0f, 1.0f, 1.0f };
	}

	void OnUpdate(Usagi::Timestep ts) override {
		// USG_INFO("FPS:{0})", 1000/ts.GetMilliseconds());

		using namespace Usagi;
		using namespace glm;

		m_Camera.Update();
		Usagi::RenderCommand::SetClearColor({ m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3] });

		// bind and clear framebuffer 
		m_Framebuffer->Bind();
		Usagi::RenderCommand::Clear();
		Usagi::Renderer::BeginScene(m_Camera);
		
		auto QuadShader = std::dynamic_pointer_cast<Usagi::OpenGLShader>(m_ShaderLibrary.Get("quad"));
		QuadShader->Bind();
		QuadShader->UploadUniformMat4("u_InverseVP", inverse(m_Camera.GetViewProjectionMatrix())); 
		m_EnvironmentIrradiance->Bind(0);
		QuadShader->UploadUniformInt("u_Texture", 0);
		Usagi::Renderer::Submit(QuadShader, m_quadVA, glm::mat4(1.0f), false);

		auto PbrShader = std::dynamic_pointer_cast<Usagi::OpenGLShader>(m_ShaderLibrary.Get("simplePBR"));
		PbrShader->Bind();
		PbrShader->UploadUniformMat4("u_ViewProjectionMatrix", m_Camera.GetViewProjectionMatrix());
		PbrShader->UploadUniformMat4("u_ModelMatrix", mat4(1.0f));
		PbrShader->UploadUniformFloat3("u_AlbedoColor", m_AlbedoInput.Color);
		PbrShader->UploadUniformFloat("u_Metalness", m_MetalnessInput.Value);
		PbrShader->UploadUniformFloat("u_Roughness", m_RoughnessInput.Value);
		PbrShader->UploadUniformFloat3("lights.Direction", m_Light.Direction);
		PbrShader->UploadUniformFloat3("lights.Radiance", m_Light.Radiance * m_LightMultiplier);
		PbrShader->UploadUniformFloat3("u_CameraPosition", m_Camera.GetPosition());
		PbrShader->UploadUniformFloat("u_RadiancePrefilter", m_RadiancePrefilter ? 1.0f : 0.0f);
		PbrShader->UploadUniformFloat("u_AlbedoTexToggle", m_AlbedoInput.UseTexture ? 1.0f : 0.0f);
		PbrShader->UploadUniformFloat("u_NormalTexToggle", m_NormalInput.UseTexture ? 1.0f : 0.0f);
		PbrShader->UploadUniformFloat("u_MetalnessTexToggle", m_MetalnessInput.UseTexture ? 1.0f : 0.0f);
		PbrShader->UploadUniformFloat("u_RoughnessTexToggle", m_RoughnessInput.UseTexture ? 1.0f : 0.0f);
		PbrShader->UploadUniformFloat("u_EnvMapRotation", m_EnvMapRotation);
		PbrShader->UploadUniformInt("u_AlbedoTexture", 1);
		PbrShader->UploadUniformInt("u_NormalTexture", 2);
		PbrShader->UploadUniformInt("u_MetalnessTexture", 3);
		PbrShader->UploadUniformInt("u_RoughnessTexture", 4);
		PbrShader->UploadUniformInt("u_EnvRadianceTex", 10);
		PbrShader->UploadUniformInt("u_EnvIrradianceTex", 11);
		PbrShader->UploadUniformInt("u_BRDFLUTTexture", 15);

		m_EnvironmentCubeMap->Bind(10);
		m_EnvironmentIrradiance->Bind(11);
		m_BRDFLUT->Bind(15);

		PbrShader->UploadUniformFloat("u_EnvRadianceTex", m_EnvMapRotation);


		PbrShader->Bind();
		if (m_AlbedoInput.TextureMap)
			m_AlbedoInput.TextureMap->Bind(1);
		if (m_NormalInput.TextureMap)
			m_NormalInput.TextureMap->Bind(2);
		if (m_MetalnessInput.TextureMap)
			m_MetalnessInput.TextureMap->Bind(3);
		if (m_RoughnessInput.TextureMap)
			m_RoughnessInput.TextureMap->Bind(4);

		//float roughness = 0.0f;
		//float x = -88.0f;
		//for (int i = 0; i < 8; i++)
		//{
		//	PbrShader->UploadUniformMat4("u_ModelMatrix", translate(mat4(1.0f), vec3(x, 0.0f, 0.0f)));
		//	PbrShader->UploadUniformFloat("u_Roughness", roughness);
		//	PbrShader->UploadUniformFloat("u_Metalness", 1.0f);
		//	Usagi::Renderer::Submit(PbrShader, m_SphereMesh->getVertexArray(), translate(mat4(1.0f), vec3(x, 0.0f, 0.0f)), true);

		//	roughness += 0.15f;
		//	x += 22.0f;
		//}

		//// Dielectrics
		//roughness = 0.0f;
		//x = -88.0f;
		//for (int i = 0; i < 8; i++)
		//{
		//	PbrShader->UploadUniformMat4("u_ModelMatrix", translate(mat4(1.0f), vec3(x, 22.0f, 0.0f)));
		//	PbrShader->UploadUniformFloat("u_Roughness", roughness);
		//	PbrShader->UploadUniformFloat("u_Metalness", 0.0f);
		//	Usagi::Renderer::Submit(PbrShader, m_SphereMesh->getVertexArray(), translate(mat4(1.0f), vec3(x, 22.0f, 0.0f)), true);

		//	roughness += 0.15f;
		//	x += 22.0f;
		//}

		Usagi::Renderer::Submit(PbrShader, m_CerberusMesh->getVertexArray(), glm::mat4(1.0f), true);



		Usagi::Renderer::EndScene();
		m_Framebuffer->Unbind();

		// ============== FRAME BUFFER =============
		m_FinalPresentBuffer->Bind();
		Usagi::RenderCommand::Clear();

		auto HDRShader = std::dynamic_pointer_cast<Usagi::OpenGLShader>(m_ShaderLibrary.Get("hdr"));
		HDRShader->Bind();
		HDRShader->UploadUniformFloat("u_Exposure", 1.0f);

		m_Framebuffer->BindTexture(0);

		Usagi::Renderer::Submit(HDRShader, m_quadVA, glm::mat4(1.0f));

		m_FinalPresentBuffer->Unbind();
		// ============== FRAME BUFFER =============
	}

	void OnImGuiRender() override {
		static bool p_open = true;
		static bool opt_fullscreen_persistant = true;
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
		bool opt_fullscreen = opt_fullscreen_persistant;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (opt_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &p_open, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Dockspace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
		}

		// Editor Panel ------------------------------------------------------------------------------
		ImGui::Begin("Settings");
		ImGui::Checkbox("Show Skybox", &m_ShowSkybox);
		// ImGui::ColorEdit3("Color##Albedo", glm::value_ptr(m_AlbedoInput.Color), ImGuiColorEditFlags_NoInputs);
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport");
		auto viewportSize = ImGui::GetContentRegionAvail();
		m_Framebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		m_FinalPresentBuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		m_Camera.SetProjectionMatrix(glm::perspectiveFov(glm::radians(45.0f), viewportSize.x, viewportSize.y, 0.1f, 10000.0f));
		ImGui::Image((void*)m_FinalPresentBuffer->GetColorAttachmentRendererID(), viewportSize, { 0, 1 }, { 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::End();
	}

private:

	Usagi::ProjectionCamera m_Camera;
	Usagi::ShaderLibrary m_ShaderLibrary;
	struct Light
	{
		glm::vec3 Direction;
		glm::vec3 Radiance;
	};
	Light m_Light;
	float m_LightMultiplier = 0.3f;

	Usagi::Ref<Usagi::Framebuffer> m_Framebuffer, m_FinalPresentBuffer;

	Usagi::Scope<Usagi::Mesh> m_SphereMesh;
	Usagi::Scope<Usagi::Mesh> m_CerberusMesh;
	Usagi::Scope<Usagi::Mesh> m_CubeMesh;

	Usagi::Ref<Usagi::VertexArray> m_quadVA;

	Usagi::Ref<Usagi::Texture2D> m_Texture_usagi3;
	Usagi::Ref<Usagi::TextureCube> m_EnvironmentCubeMap;
	Usagi::Ref<Usagi::TextureCube> m_EnvironmentIrradiance;
	Usagi::Ref<Usagi::TextureCube> m_EnvironmentSky;
	Usagi::Ref<Usagi::Texture2D> m_BRDFLUT;

	struct AlbedoInput
	{
		glm::vec3 Color = { 0.972f, 0.96f, 0.915f }; // Silver, from https://docs.unrealengine.com/en-us/Engine/Rendering/Materials/PhysicallyBased
		Usagi::Ref<Usagi::Texture2D> TextureMap;
		bool SRGB = true;
		bool UseTexture = true;
	};
	AlbedoInput m_AlbedoInput;

	struct NormalInput
	{
		Usagi::Ref<Usagi::Texture2D> TextureMap;
		bool UseTexture = true;
	};
	NormalInput m_NormalInput;

	struct MetalnessInput
	{
		float Value = 1.0f;
		Usagi::Ref<Usagi::Texture2D> TextureMap;
		bool UseTexture = true;
	};
	MetalnessInput m_MetalnessInput;

	struct RoughnessInput
	{
		float Value = 0.5f;
		Usagi::Ref<Usagi::Texture2D> TextureMap;
		bool UseTexture = true;
	};
	RoughnessInput m_RoughnessInput;

	bool m_RadiancePrefilter = false;

	float m_ClearColor[4] = { 0.3f,0.3f,0.3f,1.0f };
	bool m_ShowSkybox = true;
	float m_EnvMapRotation = 0.0f;
};