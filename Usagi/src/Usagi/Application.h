#pragma once

#include "Core.h"

#include "Window.h"
#include "Usagi/LayerStack.h"
#include "Events/Event.h"
#include "Usagi/Events/ApplicationEvent.h"
#include "Usagi/ImGui/ImGuiLayer.h"

#include "Usagi/Renderer/Shader.h"
#include "Usagi/Renderer/Buffer.h"
namespace Usagi {

	class USAGI_API Application
	{
	public:

		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		// call Application::Get() to get the reference of the Application first.
		static inline Application& Get() { return *s_Instance; }
		// then call Application.GetWindow() to get the reference of the Window.
		inline Window& GetWindow() { return  *m_Window; }

	private:

		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		// static Application pointer, means only one application should exists.
		static Application* s_Instance;

		bool m_Running = true;

		unsigned int m_VertexArray;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
	};

	// To be defined in CLIENT, and should be called by CLIENT
	Application* CreateApplication();
}


