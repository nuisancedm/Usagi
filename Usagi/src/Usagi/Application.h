#pragma once

#include "Core.h"

#include "Window.h"
#include "Usagi/LayerStack.h"
#include "Events/Event.h"
#include "Usagi/Events/ApplicationEvent.h"
#include "Usagi/ImGui/ImGuiLayer.h"

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

		static inline Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return  *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}


