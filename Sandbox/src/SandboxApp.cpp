#include <Usagi.h>

#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>

#include "platform/OpenGL/OpenGLShader.h"
#include "glm/gtc/type_ptr.hpp"


class ExampleLayer : public Usagi::Layer {
public:
	ExampleLayer()
		:Layer("Example") , 
		m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), 
		m_CameraPosition(0.0f)
	{	
		float vertices[3 * 7] = {                                            //@@ vertices data
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};
		unsigned int indices[3] = { 0,1,2 };                                 //@@ indice data  


		m_VertexArray.reset(Usagi::VertexArray::create());                          //@@ reset VA smart pointer, VA is a member of Application.

		Usagi::Ref<Usagi::VertexBuffer> vertexBuffer;                          //@@ init VB's smart pointer, VB is not a member of Application
		vertexBuffer.reset(Usagi::VertexBuffer::Create(vertices, sizeof(vertices)));//@@ reset VB smart pointer
		Usagi::BufferLayout layout =                                                //@@ define the layout of the VB
		{
			{Usagi::ShaderDataType::Float3, "a_Position"},
			{Usagi::ShaderDataType::Float4, "a_Color"}
		};
		vertexBuffer->SetLayout(layout);                                     //@@ set the layout for VB
		m_VertexArray->AddVertexBuffer(vertexBuffer);                        //@@ feed the VB to VA


		Usagi::Ref<Usagi::IndexBuffer> indexBuffer;                            //@@ init IB's smart pointer, IB is not a member of Application
		indexBuffer.reset(Usagi::IndexBuffer::Create(indices, 3));                  //@@ reset IB smart pointer
		m_VertexArray->SetIndexBuffer(indexBuffer);                          //@@ feed the IB to VA


		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};

		unsigned int squareIndices[6] = { 0,1,2,2,3,0 };

		m_SquareVA.reset(Usagi::VertexArray::create());

		Usagi::Ref<Usagi::VertexBuffer> squareVB;
		squareVB.reset(Usagi::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		Usagi::BufferLayout squareLayout = {
			{Usagi::ShaderDataType::Float3, "a_Position"},
			{Usagi::ShaderDataType::Float2, "a_TexCoord"}
		};
		squareVB->SetLayout(squareLayout);
		m_SquareVA->AddVertexBuffer(squareVB);

		Usagi::Ref<Usagi::IndexBuffer> squareIB;
		squareIB.reset(Usagi::IndexBuffer::Create(squareIndices, 6));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSource = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;		
			out vec4 v_Color;		
	
	
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";
		std::string fragmentSource = R"(
			#version 330 core
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;
	
			void main()
			{
				color = v_Color;
			}
		)";

		m_Shader = Usagi::Shader::create("VertexPosColor", vertexSource, fragmentSource);

		std::string flatColorVertexSource = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;		
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			
			
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(v_Position, 1.0);
			}
		)";

		std::string flatColorFragmentSource = R"(
			#version 330 core
			layout(location = 0) out vec4 color;
			
			in vec3 v_Position;
			uniform	vec3 u_Color;		

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		m_flatColorShader = Usagi::Shader::create("FlatColor", flatColorVertexSource, flatColorFragmentSource);

		auto TextureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = Usagi::Texture2D::Create("assets/textures/usagi3.png");
		m_Texture2 = Usagi::Texture2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<Usagi::OpenGLShader>(TextureShader)->Bind();
		std::dynamic_pointer_cast<Usagi::OpenGLShader>(TextureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Usagi::Timestep ts) override {

		/*USG_TRACE("Delta time:{0} s, ({1} ms)", ts.GetSeconds(), ts.GetMilliseconds());*/

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

		std::dynamic_pointer_cast<Usagi::OpenGLShader>(m_flatColorShader)->Bind();
		std::dynamic_pointer_cast<Usagi::OpenGLShader>(m_flatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);
		

		for (int y = 0; y < 10; y++) {
			for (int i = 0; i < 10; i++) {
				glm::vec3 pos(i * 0.11f, y*0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Usagi::Renderer::Submit(m_flatColorShader, m_SquareVA, transform);
			}
		}

		auto TextureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Usagi::Renderer::Submit(TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_Texture2->Bind();
		Usagi::Renderer::Submit(TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Triangle
		// Usagi::Renderer::Submit(m_Shader, m_VertexArray);

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

	/*bool OnKeyPressedEvent(Usagi::KeyPressedEvent& event) {
		if (event.GetKeyCode() == USG_KEY_LEFT) 
			m_CameraPosition.x -= m_CameraSpeed;
	
		if (event.GetKeyCode() == USG_KEY_RIGHT) 
			m_CameraPosition.x += m_CameraSpeed;

		if (event.GetKeyCode() == USG_KEY_DOWN)
			m_CameraPosition.y -= m_CameraSpeed;

		if (event.GetKeyCode() == USG_KEY_UP)
			m_CameraPosition.y += m_CameraSpeed;
		
		
		return false;
	}*/

private:
	Usagi::ShaderLibrary m_ShaderLibrary;
	Usagi::Ref<Usagi::Shader> m_Shader;					    //@@ shader and VAs
	Usagi::Ref<Usagi::VertexArray> m_VertexArray;

	Usagi::Ref<Usagi::Shader> m_flatColorShader;
	Usagi::Ref<Usagi::VertexArray> m_SquareVA;

	Usagi::Ref<Usagi::Texture2D> m_Texture;
	Usagi::Ref<Usagi::Texture2D> m_Texture2;

	Usagi::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition = { 0.0f,0.0f,0.0f };
	float m_CameraRotation = 0.0f;
	
	float m_CameraMoveSpeed = 5.0f; // unit per second
	float m_CameraRotationSpeed = 180.0f; // degree per second

	glm::vec3 m_SquareColor = { 0.2f,0.3f,0.8f };


};


class Sandbox :public Usagi::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {}

};

Usagi::Application* Usagi::CreateApplication() {
	return new Sandbox();
}