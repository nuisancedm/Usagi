#pragma once

#include "Core.h"

#include "Window.h"
#include "Usagi/LayerStack.h"
#include "Events/Event.h"
#include "Usagi/Events/ApplicationEvent.h"
#include "Usagi/ImGui/ImGuiLayer.h"

#include "Usagi/Core/Timestep.h"


namespace Usagi {

	class USAGI_API Application
	{
	public:

		Application();											 //@@ The Constructor: create window, set the scene, create datas, shaders, buffers etc.
		virtual ~Application();                                  //@@ The Destructor: not implemented yet. 

		void Run();                                              //@@ Render loop: update all layers.
		void OnEvent(Event& e);                                  //@@ Event function defined by application, it will be passed to Window and called by Window to inform all layers in the application.

		void PushLayer(Layer* layer);                            //@@ Push a layer to the LayerStack
		void PushOverlay(Layer* overlay);					     //@@ Push a Overlay to the LayerStack

		static inline Application& Get() { return *s_Instance; } //@@ call Application::Get() to get the reference of the Application first.
		inline Window& GetWindow() { return  *m_Window; }        //@@ then call Application.GetWindow() to get the reference of the Window.    

															     //============== C++ NOTES HERE =================
															     // class Application and class Window are both very complicated class.
															     // the return type is a reference here to avoid duplicate of the huge class in memory.

	private:
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		std::unique_ptr<Window> m_Window;                        //@@ Application create and hold the Window.
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;                         //@@ static Application pointer, means only one application should exists.
													            //============== C++ NOTES HERE =================
															    // static property need a redefine outside the class
																// static property belongs not the instance of the class but the class it self.
		                                                        // all the instances share only one copy of the static property member.
		bool m_Running = true;                                  //@@ flag for running loop


	};

	// To be defined in CLIENT, and should be called by CLIENT
	Application* CreateApplication();
}


