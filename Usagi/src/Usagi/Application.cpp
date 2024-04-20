#include "usgpch.h"
#include "Application.h"

#include "Usagi/Events/ApplicationEvent.h"
#include "Usagi/Log.h"

#include <GLFW/glfw3.h>
namespace Usagi {
	// 非静态成员函数的调用需要实例来调用，即实例名.函数名()的形式调用
	// 但是当非静态成员函数需要传入某个函数成为回调函数时，它没法使用实例名.函数名()的形式调用。
	// 需要用std::bind()来为这个函数指定上下文(即这个函数和那个实例绑定)，std::bind会返回一个基于这个函数新的可调用对象 std::function
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application::Application() {
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application() {
		
	}

	void Application::OnEvent(Event& e) {
		USG_CORE_INFO("{0}",e);
	};

	void Application::Run() {
		while (m_Running) {
			glClearColor(1,0,1,1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		};
	}
}