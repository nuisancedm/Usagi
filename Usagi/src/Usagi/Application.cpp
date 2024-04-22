#include "usgpch.h"
#include "Application.h"

#include "Usagi/Log.h"

#include <GLFW/glfw3.h>
namespace Usagi {
	// �Ǿ�̬��Ա�����ĵ�����Ҫʵ�������ã���ʵ����.������()����ʽ����
	// ���ǵ��Ǿ�̬��Ա������Ҫ����ĳ��������Ϊ�ص�����ʱ����û��ʹ��ʵ����.������()����ʽ���á�
	// ��Ҫ��std::bind()��Ϊ�������ָ��������(������������Ǹ�ʵ����)��std::bind�᷵��һ��������������µĿɵ��ö��� std::function
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application::Application() {
		// ����һ��Window
		m_Window = std::unique_ptr<Window>(Window::Create());
		// ΪWindow�����¼��ص���������һ������ָ�룬������һ������ʱ�������������
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