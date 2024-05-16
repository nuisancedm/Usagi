#include "usgpch.h"
#include "Application.h"

#include "Usagi/Log.h"

#include <glad/glad.h>
#include "Input.h"
namespace Usagi 
{
	// outside definition of a static class member s_Instance
	Application* Application::s_Instance = nullptr;


	// constructor
	Application::Application() {
		// if s_Instance is not nullptr, means applicaiton already exists.
		USG_CORE_ASSERT(s_Instance, "Application already exists!")
		// point s_Instance to the instance.
		s_Instance = this;

		// initialize m_Window, and set the callback function for the window.
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(USG_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		
		m_VertexArray.reset(VertexArray::create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout = 
		{
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"}
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		unsigned int indices[3] = { 0,1,2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, 3));
		m_VertexArray->SetIndexBuffer(indexBuffer);


		m_SquareVA.reset(VertexArray::create());
		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,		
			-0.75f,  0.75f, 0.0f
		};
		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		BufferLayout squareLayout = {
			{ShaderDataType::Float3, "a_Position"}
		};
		squareVB->SetLayout(squareLayout);
		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int squareIndices[6] = { 0,1,2,2,3,0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, 6));
		m_SquareVA->SetIndexBuffer(squareIB);



		std::string vertexSource = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;		
			out vec4 v_Color;		
	
	
			void main()
			{
				gl_Position = vec4(a_Position, 1.0);
				v_Position = a_Position;
				v_Color = a_Color;
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
	
			void main()
			{
				gl_Position = vec4(a_Position, 1.0);
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
			glClearColor(0.15,0.15,0.15,1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_PureBlueShader->Bind();
			m_SquareVA->Bind();
			glDrawElements(GL_TRIANGLES, m_SquareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			m_Shader->Bind();
			m_VertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

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