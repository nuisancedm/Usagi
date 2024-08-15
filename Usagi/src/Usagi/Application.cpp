#include "usgpch.h"
#include "Application.h"

#include "Usagi/Log.h"

#include <glad/glad.h>
#include "Usagi/Renderer/Renderer.h"
#include "Input.h"

namespace Usagi 
{
	
	Application* Application::s_Instance = nullptr;                          //@@ outside definition of a static class member s_Instance

	Application::Application() 
		:m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		
		USG_CORE_ASSERT(s_Instance, "Application already exists!")           //@@ if s_Instance is not nullptr, means applicaiton already exists.
		s_Instance = this;                                                   //@@ point s_Instance to the instance.

		
		m_Window = std::unique_ptr<Window>(Window::Create());                //@@ initialize m_Window, and set the callback function for the window.      
		m_Window->SetEventCallback(USG_BIND_EVENT_FN(Application::OnEvent)); //@@ pass the OnEvent function to Window

		m_ImGuiLayer = new ImGuiLayer();                                     //@@ create imgui layer, which is an overlay
		PushOverlay(m_ImGuiLayer);

		
		float vertices[3 * 7] = {                                            //@@ vertices data
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};
		unsigned int indices[3] = { 0,1,2 };                                 //@@ indice data  

		
		m_VertexArray.reset(VertexArray::create());                          //@@ reset VA smart pointer, VA is a member of Application.

		std::shared_ptr<VertexBuffer> vertexBuffer;                          //@@ init VB's smart pointer, VB is not a member of Application
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));//@@ reset VB smart pointer
		BufferLayout layout =                                                //@@ define the layout of the VB
		{
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"}
		};
		vertexBuffer->SetLayout(layout);                                     //@@ set the layout for VB
		m_VertexArray->AddVertexBuffer(vertexBuffer);                        //@@ feed the VB to VA


		std::shared_ptr<IndexBuffer> indexBuffer;                            //@@ init IB's smart pointer, IB is not a member of Application
		indexBuffer.reset(IndexBuffer::Create(indices, 3));                  //@@ reset IB smart pointer
		m_VertexArray->SetIndexBuffer(indexBuffer);                          //@@ feed the IB to VA


		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};
		unsigned int squareIndices[6] = { 0,1,2,2,3,0 };

		m_SquareVA.reset(VertexArray::create());
		
		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		BufferLayout squareLayout = {
			{ShaderDataType::Float3, "a_Position"}
		};
		squareVB->SetLayout(squareLayout);
		m_SquareVA->AddVertexBuffer(squareVB);

		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, 6));
		m_SquareVA->SetIndexBuffer(squareIB);



		std::string vertexSource = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;		
			out vec4 v_Color;		
	
	
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

		m_Shader.reset(new Shader(vertexSource, fragmentSource));

		std::string pureBlueVertexSource = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;		
			
			uniform mat4 u_ViewProjection;
			
			
			void main()
			{
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

		m_PureBlueShader.reset(new Shader(pureBlueVertexSource, pureBlueFragmentSource));
	}

	Application::~Application() {
		
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(USG_BIND_EVENT_FN(Application::OnWindowClose));
		// USG_CORE_TRACE("{0}",e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	};

	void Application::Run() {
		while (m_Running) {

			RenderCommand::SetClearColor({ 0.15, 0.15, 0.15, 1 });
			RenderCommand::Clear();

			m_Camera.SetPosition({ 0.5f,0.5f,0.0f });
			m_Camera.SetRotation(45.0f);

			Renderer::BeginScene(m_Camera);
			
			Renderer::Submit(m_PureBlueShader, m_SquareVA);
			Renderer::Submit(m_Shader, m_VertexArray);
			
			Renderer::EndScene();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			{

				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		};
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}
}