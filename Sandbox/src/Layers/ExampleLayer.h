#pragma once
#include <Usagi.h>
#include "platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"
#include <glm/glm.hpp>

class ExampleLayer : public Usagi::Layer {
public:
	ExampleLayer()
		:Layer("Example"),
		m_Camera(-1.6f, 1.6f, -0.9f, 0.9f),
		m_CameraPosition(0.0f)
	{
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};
		unsigned int indices[3] = { 0,1,2 };

		m_VertexArray = Usagi::VertexArray::create();

		Usagi::Ref<Usagi::VertexBuffer> vertexBuffer = Usagi::VertexBuffer::Create(vertices, sizeof(vertices));
		Usagi::BufferLayout layout =
		{
			{Usagi::ShaderDataType::Float3, "a_Position"},
			{Usagi::ShaderDataType::Float4, "a_Color"}
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		Usagi::Ref<Usagi::IndexBuffer> indexBuffer = Usagi::IndexBuffer::Create(indices, 3);
		m_VertexArray->SetIndexBuffer(indexBuffer);


		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};

		unsigned int squareIndices[6] = { 0,1,2,2,3,0 };

		m_SquareVA = Usagi::VertexArray::create();

		Usagi::Ref<Usagi::VertexBuffer> squareVB = Usagi::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

		Usagi::BufferLayout squareLayout = {
			{Usagi::ShaderDataType::Float3, "a_Position"},
			{Usagi::ShaderDataType::Float2, "a_TexCoord"}
		};
		squareVB->SetLayout(squareLayout);
		m_SquareVA->AddVertexBuffer(squareVB);

		Usagi::Ref<Usagi::IndexBuffer> squareIB = Usagi::IndexBuffer::Create(squareIndices, 6);
		m_SquareVA->SetIndexBuffer(squareIB);

		m_ShaderLibrary.Load("assets/shaders/FlatColor.glsl");
		m_ShaderLibrary.Load("assets/shaders/TriangleVertexColor.glsl");
		auto TextureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = Usagi::Texture2D::Create("assets/textures/usagi3.png");
		m_Texture2 = Usagi::Texture2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<Usagi::OpenGLShader>(TextureShader)->Bind();
		std::dynamic_pointer_cast<Usagi::OpenGLShader>(TextureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Usagi::Timestep ts) override {

		// USG_TRACE("Delta time:{0} s, ({1} ms)", ts.GetSeconds(), ts.GetMilliseconds());
		// USG_INFO("ExampleLayer::Update")

		if (Usagi::Input::IsKeyPressed(USG_KEY_TAB))
			USG_TRACE("Tab Key is pressed (polling)");

		if (Usagi::Input::IsKeyPressed(USG_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (Usagi::Input::IsKeyPressed(USG_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (Usagi::Input::IsKeyPressed(USG_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		else if (Usagi::Input::IsKeyPressed(USG_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;

		if (Usagi::Input::IsKeyPressed(USG_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		else if (Usagi::Input::IsKeyPressed(USG_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed * ts;


		Usagi::RenderCommand::SetClearColor({ 0.15, 0.15, 0.15, 1 });
		Usagi::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Usagi::Renderer::BeginScene(m_Camera);

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));

		auto flatColorShader = m_ShaderLibrary.Get("FlatColor");
		auto TextureShader = m_ShaderLibrary.Get("Texture");
		auto TriangleVertexColorShader = m_ShaderLibrary.Get("TriangleVertexColor");

		std::dynamic_pointer_cast<Usagi::OpenGLShader>(flatColorShader)->Bind();
		std::dynamic_pointer_cast<Usagi::OpenGLShader>(flatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);


		// Tile map
		for (int y = 0; y < 10; y++) {
			for (int i = 0; i < 10; i++) {
				glm::vec3 pos(i * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Usagi::Renderer::Submit(flatColorShader, m_SquareVA, transform);
			}
		}

		// usagi img
		m_Texture->Bind();
		Usagi::Renderer::Submit(TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// C logo img
		m_Texture2->Bind();
		Usagi::Renderer::Submit(TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Triangle
		Usagi::Renderer::Submit(TriangleVertexColorShader, m_VertexArray);

		Usagi::Renderer::EndScene();

	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Hello World");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Usagi::Event& event) override {
		//USG_TRACE("{0}", event);
		/*if (event.GetEventType() == Usagi::EventType::KeyPressed)
		{
			Usagi::KeyPressedEvent& e = (Usagi::KeyPressedEvent&)event;
			if (e.GetKeyCode() == USG_KEY_TAB) {
				USG_TRACE("Tab Key is pressed (event)");
			}
			USG_TRACE("{0}", (char)e.GetKeyCode());
		}*/

		/*Usagi::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Usagi::KeyPressedEvent>(USG_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));*/
	}

	bool OnKeyPressedEvent(Usagi::KeyPressedEvent& event) {
		if (event.GetKeyCode() == USG_KEY_LEFT)
			m_CameraPosition.x -= m_CameraMoveSpeed;

		if (event.GetKeyCode() == USG_KEY_RIGHT)
			m_CameraPosition.x += m_CameraMoveSpeed;

		if (event.GetKeyCode() == USG_KEY_DOWN)
			m_CameraPosition.y -= m_CameraMoveSpeed;

		if (event.GetKeyCode() == USG_KEY_UP)
			m_CameraPosition.y += m_CameraMoveSpeed;


		return false;
	}

private:
	Usagi::ShaderLibrary m_ShaderLibrary;

	Usagi::Ref<Usagi::VertexArray> m_VertexArray;
	Usagi::Ref<Usagi::VertexArray> m_SquareVA;

	Usagi::Ref<Usagi::Texture2D> m_Texture;
	Usagi::Ref<Usagi::Texture2D> m_Texture2;

	Usagi::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition = { 0.0f,0.0f,0.0f };
	float m_CameraRotation = 0.0f;

	float m_CameraMoveSpeed = 5.0f;
	float m_CameraRotationSpeed = 180.0f;

	glm::vec3 m_SquareColor = { 0.2f,0.3f,0.8f };


};