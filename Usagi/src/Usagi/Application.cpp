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

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);


		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		
		

		unsigned int indices[3] = { 0,1,2 };

		/*glGenBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)));


		std::string vertexSource = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			out vec3 v_Position;		
	
	
			void main()
			{
				gl_Position = vec4(a_Position, 1.0);
				v_Position = a_Position;
			}
		)";
		std::string fragmentSource = R"(
			#version 330 core
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
	
			void main()
			{
				color = vec4(v_Position*0.5+0.5,1.0);
			}
		)";
		
		m_Shader.reset(new Shader(vertexSource, fragmentSource));


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

			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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