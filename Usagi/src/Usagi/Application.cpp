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
	{
		
		USG_CORE_ASSERT(s_Instance, "Application already exists!")           //@@ if s_Instance is not nullptr, means applicaiton already exists.
		s_Instance = this;                                                   //@@ point s_Instance to the instance.

		
		m_Window = std::unique_ptr<Window>(Window::Create());                //@@ initialize m_Window, and set the callback function for the window.      
		m_Window->SetEventCallback(USG_BIND_EVENT_FN(Application::OnEvent)); //@@ pass the OnEvent function to Window

		m_ImGuiLayer = new ImGuiLayer();                                     //@@ create imgui layer, which is an overlay
		PushOverlay(m_ImGuiLayer);

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