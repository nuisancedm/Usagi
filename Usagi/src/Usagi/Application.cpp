#include "usgpch.h"
#include "Application.h"

#include "Usagi/Log.h"

#include <GLFW/glfw3.h>
namespace Usagi {
	// 非静态成员函数的调用需要实例来调用，即实例名.函数名()的形式调用
	// 但是当非静态成员函数需要传入某个函数成为回调函数时，它没法使用实例名.函数名()的形式调用。
	// 需要用std::bind()来为这个函数指定上下文(即这个函数和那个实例绑定)，std::bind会返回一个基于这个函数新的可调用对象 std::function
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application::Application() {
		// 创建一个Window
		m_Window = std::unique_ptr<Window>(Window::Create());
		// 为Window设置事件回调函数，是一个函数指针，当满足一定条件时，调用这个函数
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application() {
		
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		// USG_CORE_TRACE("{0}",e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	};

	void Application::Run() {
		while (m_Running) {
			glClearColor(0,0,0,1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		};
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}
}