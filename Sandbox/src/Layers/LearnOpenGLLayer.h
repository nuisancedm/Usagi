#pragma once
#include <Usagi.h>
#include "platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"
#include <glm/glm.hpp>

class LearnOpenGL : public Usagi::Layer {
public:

	LearnOpenGL()
		:m_Camera(glm::perspectiveFov(glm::radians(45.0f), 1280.0f, 720.0f, 0.1f, 10000.0f)),
		m_PointLight(glm::vec3(30.0f,30.0f,30.0f), glm::vec3(1.0f,1.0f,1.0f), 1.0f)
	{
		// quad vertices and indices for framebuffer;
		float quadVertices[5 * 4] = {
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		};
		unsigned int quadIndices[6] = { 0,1,2,2,3,0 };

		// create quadVA, VA is a member
		m_quadVA = Usagi::VertexArray::create();
		// create VB VBLayout IB only active in the scope 
		Usagi::Ref<Usagi::VertexBuffer> vertexBuffer = Usagi::VertexBuffer::Create(quadVertices, sizeof(quadVertices));
		Usagi::BufferLayout quadLayout = {
			{Usagi::ShaderDataType::Float3, "a_Position"},
			{Usagi::ShaderDataType::Float2, "a_TexCoord"}
		};
		vertexBuffer->SetLayout(quadLayout);
		Usagi::Ref<Usagi::IndexBuffer> indexBuffer = Usagi::IndexBuffer::Create(quadIndices, 6);
		// add VB IB for VA
		m_quadVA->AddVertexBuffer(vertexBuffer);
		m_quadVA->SetIndexBuffer(indexBuffer);

		// create framebuffer
		m_Framebuffer = Usagi::Framebuffer::Create(1280, 720, Usagi::FramebufferFormat::RGBA16F);
		m_FinalPresentBuffer = Usagi::Framebuffer::Create(1280, 720, Usagi::FramebufferFormat::RGBA8);

		// load Shaders
		m_ShaderLibrary.Load("assets/shaders/first3D.glsl");
		m_ShaderLibrary.Load("assets/shaders/hdr.glsl");
		m_ShaderLibrary.Load("assets/shaders/LearnOpenGLShaders/lightShader.glsl");
		m_ShaderLibrary.Load("assets/shaders/LearnOpenGLShaders/PhongShader.glsl");
		m_ShaderLibrary.Load("assets/shaders/LearnOpenGLShaders/Skybox.glsl");

		// load Textures
		m_Texture_usagi3 = Usagi::Texture2D::Create("assets/textures/usagi3.png");
		m_Texture_Cerberus_A = Usagi::Texture2D::Create("assets/textures/cerberus/cerberus_A.png");
		m_Texture_Cerberus_R = Usagi::Texture2D::Create("assets/textures/cerberus/cerberus_R.png");
		std::vector<std::string> cubepaths= {
			"assets/textures/skybox/right.jpg",
			"assets/textures/skybox/left.jpg",
			"assets/textures/skybox/top.jpg",
			"assets/textures/skybox/bottom.jpg",
			"assets/textures/skybox/front.jpg",
			"assets/textures/skybox/back.jpg"
		};
		// m_CubeTexture_Pinetree = Usagi::TextureCube::Create("assets/textures/skybox_pinetree/Arches_E_PineTree_Radiance.tga");
		m_CubeTexture_Pinetree = Usagi::TextureCube::Create(cubepaths);

		// Load Meshs
		m_SphereMesh.reset(new Usagi::Mesh("assets/models/Sphere.fbx"));
		m_CerberusMesh.reset(new Usagi::Mesh("assets/models/Cerberus.fbx"));
		m_CubeMesh.reset(new Usagi::Mesh("assets/models/Cube.fbx"));
	}

	void OnUpdate(Usagi::Timestep ts) override {
		// USG_INFO("FPS:{0})", 1000/ts.GetMilliseconds());
		Usagi::RenderCommand::SetClearColor({ m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3] });

		//update camera
		m_Camera.Update();

		// bind and clear framebuffer 
		m_Framebuffer->Bind();
		Usagi::RenderCommand::Clear();

		Usagi::Renderer::BeginScene(m_Camera);

		glm::mat4 transformMatrix_Sphere = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 100.0f)) *
									 glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
									 glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));

		glm::mat4 transformMatrix_Cerberus = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)) *
										glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
										glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));

		glm::mat4 transformMatrix_rectangle = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 50.0f, 50.0f)) *
										glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
										glm::scale(glm::mat4(1.0f), glm::vec3(50.0f));

		glm::mat4 transformMatrix_Light = glm::translate(glm::mat4(1.0f), m_PointLight.getPosition()) *
										glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
										glm::scale(glm::mat4(1.0f), glm::vec3(0.3f));

		// bind uniform for a shader
		auto lightShader = std::dynamic_pointer_cast<Usagi::OpenGLShader>(m_ShaderLibrary.Get("lightShader"));
		lightShader->Bind();
		lightShader->UploadUniformFloat3("u_lightColor", m_PointLight.getColor());

		auto PhongShader = std::dynamic_pointer_cast<Usagi::OpenGLShader>(m_ShaderLibrary.Get("PhongShader")); 
		PhongShader->Bind();
		PhongShader->UploadUniformFloat3("u_viewPosition", m_Camera.GetPosition());
		PhongShader->UploadUniformMat3("u_NormalMatrix", glm::mat3(glm::transpose(glm::inverse(transformMatrix_Cerberus))));
		PhongShader->UploadUniformFloat3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
		PhongShader->UploadUniformInt("material.diffuse", 0);
		PhongShader->UploadUniformInt("material.specular", 1);
		PhongShader->UploadUniformFloat("material.shininess", 32);
		PhongShader->UploadUniformFloat3("pointLight.position", m_PointLight.getPosition());
		PhongShader->UploadUniformFloat3("pointLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		PhongShader->UploadUniformFloat3("pointLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		PhongShader->UploadUniformFloat3("pointLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		PhongShader->UploadUniformFloat3("pointLight.attenuationParams", m_PointLight.getAttenuation());

		auto skyboxShader = std::dynamic_pointer_cast<Usagi::OpenGLShader>(m_ShaderLibrary.Get("Skybox"));
		skyboxShader->Bind();
		skyboxShader->UploadUniformInt("skybox", 2);
		skyboxShader->UploadUniformMat4("u_View", glm::mat4(glm::mat3(m_Camera.GetViewMatrix())));
		skyboxShader->UploadUniformMat4("u_Projection", m_Camera.GetProjectionMatrix());

		// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% BEGIN SCENE %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		{
			m_Texture_Cerberus_A->Bind(0);
			m_Texture_Cerberus_R->Bind(1);
			m_CubeTexture_Pinetree->Bind(2);
			
			// Cube
			Usagi::RenderCommand::SetDepthMask(false);
			Usagi::Renderer::Submit(m_ShaderLibrary.Get("Skybox"), m_CubeMesh->getVertexArray(), glm::mat4(1.0f));
			Usagi::RenderCommand::SetDepthMask(true);

			// Sphere
			// Usagi::Renderer::Submit(m_ShaderLibrary.Get("PhongShader"), m_SphereMesh->getVertexArray(), transformMatrix_Sphere);
			
			// Cerberus
			 Usagi::Renderer::Submit(m_ShaderLibrary.Get("PhongShader"), m_CerberusMesh->getVertexArray(), transformMatrix_Cerberus);
			
			// Light
			Usagi::Renderer::Submit(m_ShaderLibrary.Get("lightShader"), m_SphereMesh->getVertexArray(), transformMatrix_Light);
		}
		// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  END SCENE  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
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
		ImGui::ColorEdit3("Clear Color", m_ClearColor);
		ImGui::Separator();
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
	Usagi::PointLight m_PointLight;

	Usagi::Ref<Usagi::Framebuffer> m_Framebuffer, m_FinalPresentBuffer;

	Usagi::Scope<Usagi::Mesh> m_SphereMesh;
	Usagi::Scope<Usagi::Mesh> m_CerberusMesh;
	Usagi::Scope<Usagi::Mesh> m_CubeMesh;

	Usagi::Ref<Usagi::VertexArray> m_quadVA;

	Usagi::Ref<Usagi::Texture2D> m_Texture_usagi3;
	Usagi::Ref<Usagi::Texture2D> m_Texture_Cerberus_A;
	Usagi::Ref<Usagi::Texture2D> m_Texture_Cerberus_R;
	Usagi::Ref<Usagi::TextureCube> m_CubeTexture_Pinetree;

	float m_ClearColor[4] = { 0.1f,0.1f,0.1f,1.0f };
};