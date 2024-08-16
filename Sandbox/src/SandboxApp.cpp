#include <Usagi.h>
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>

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

		std::shared_ptr<Usagi::VertexBuffer> vertexBuffer;                          //@@ init VB's smart pointer, VB is not a member of Application
		vertexBuffer.reset(Usagi::VertexBuffer::Create(vertices, sizeof(vertices)));//@@ reset VB smart pointer
		Usagi::BufferLayout layout =                                                //@@ define the layout of the VB
		{
			{Usagi::ShaderDataType::Float3, "a_Position"},
			{Usagi::ShaderDataType::Float4, "a_Color"}
		};
		vertexBuffer->SetLayout(layout);                                     //@@ set the layout for VB
		m_VertexArray->AddVertexBuffer(vertexBuffer);                        //@@ feed the VB to VA


		std::shared_ptr<Usagi::IndexBuffer> indexBuffer;                            //@@ init IB's smart pointer, IB is not a member of Application
		indexBuffer.reset(Usagi::IndexBuffer::Create(indices, 3));                  //@@ reset IB smart pointer
		m_VertexArray->SetIndexBuffer(indexBuffer);                          //@@ feed the IB to VA


		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};
		unsigned int squareIndices[6] = { 0,1,2,2,3,0 };

		m_SquareVA.reset(Usagi::VertexArray::create());

		std::shared_ptr<Usagi::VertexBuffer> squareVB;
		squareVB.reset(Usagi::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		Usagi::BufferLayout squareLayout = {
			{Usagi::ShaderDataType::Float3, "a_Position"}
		};
		squareVB->SetLayout(squareLayout);
		m_SquareVA->AddVertexBuffer(squareVB);

		std::shared_ptr<Usagi::IndexBuffer> squareIB;
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

		m_Shader.reset(new Usagi::Shader(vertexSource, fragmentSource));

		std::string pureBlueVertexSource = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;		
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			
			
			void main()
			{
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string pureBlueFragmentSource = R"(
			#version 330 core
			layout(location = 0) out vec4 color;
	
			void main()
			{
				color = vec4(0.2f, 0.3f, 1.0f, 1.0f);
			}
		)";

		m_PureBlueShader.reset(new Usagi::Shader(pureBlueVertexSource, pureBlueFragmentSource));
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


		for (int y = 0; y < 10; y++) {
			for (int i = 0; i < 10; i++) {
				glm::vec3 pos(i * 0.11f, y*0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Usagi::Renderer::Submit(m_PureBlueShader, m_SquareVA, transform);
			}
		}

		Usagi::Renderer::Submit(m_Shader, m_VertexArray);

		


		Usagi::Renderer::EndScene();
		
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
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
	std::shared_ptr<Usagi::Shader> m_Shader;					    //@@ shader and VAs
	std::shared_ptr<Usagi::VertexArray> m_VertexArray;

	std::shared_ptr<Usagi::Shader> m_PureBlueShader;
	std::shared_ptr<Usagi::VertexArray> m_SquareVA;

	Usagi::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition = { 0.0f,0.0f,0.0f };
	float m_CameraRotation = 0.0f;
	
	float m_CameraMoveSpeed = 5.0f; // unit per second
	float m_CameraRotationSpeed = 180.0f; // degree per second


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