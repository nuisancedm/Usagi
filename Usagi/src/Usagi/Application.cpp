#include "Application.h"

#include "Usagi/Events/ApplicationEvent.h"
#include "Usagi/Log.h"

namespace Usagi {
	Application::Application() {
		
	}

	Application::~Application() {
		
	}

	void Application::Run() {
		WindwoResizeEvent e (1280, 720);
		USG_TRACE(e);
		while (true);
	}
}