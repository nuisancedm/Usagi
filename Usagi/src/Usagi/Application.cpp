#include "usgpch.h"
#include "Application.h"

#include "Usagi/Events/ApplicationEvent.h"
#include "Usagi/Log.h"

#include <GLFW/glfw3.h>
namespace Usagi {
	// �Ǿ�̬��Ա�����ĵ�����Ҫʵ�������ã���ʵ����.������()����ʽ����
	// ���ǵ��Ǿ�̬��Ա������Ҫ����ĳ��������Ϊ�ص�����ʱ����û��ʹ��ʵ����.������()����ʽ���á�
	// ��Ҫ��std::bind()��Ϊ�������ָ��������(������������Ǹ�ʵ����)��std::bind�᷵��һ��������������µĿɵ��ö��� std::function
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