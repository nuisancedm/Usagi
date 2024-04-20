#include "usgpch.h"
#include "WindowsWindow.h"

#include "Usagi/Events/ApplicationEvent.h"
#include "Usagi/Events/KeyEvent.h"
#include "Usagi/Events/MouseEvent.h"


namespace Usagi {
	static bool s_GLFWInitialized = false;

	// Window* Window::Createʵ�֣�����һ��WindowsWindow��ʵ���ĵ�ַ
	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	// WindowsWindow ���캯��������WindowsWindow��˽�з���Init
	WindowsWindow::WindowsWindow(const WindowProps& props) {
		Init(props);
	}

	// WindowsWindow �⹹����������WindowsWindow��˽�з���ShutDown
	WindowsWindow::~WindowsWindow() {
		ShutDown();
	}

	// WindowsWindow˽�з���Init�ľ���ʵ��
	void WindowsWindow::Init(const WindowProps& props) {
		// m_Data��ʼ��
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		// core info
		USG_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);


		if (!s_GLFWInitialized) {
			// ����glfwInit()������һ��״̬�룬����glfw��ʼ���Ƿ�ɹ�
			int success = glfwInit();
			// ������Ч��ǰ���ǿ�����ASSERT
			USG_CORE_ASSERT(success, "Could not initialize GLFW!");
			s_GLFWInitialized = true;
		}

		// ����glfw����
		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		// ����������һ���û�ָ�룬���ָ��ָ��һƬ���ݣ�����Ƭ���ݹ������������
		glfwSetWindowUserPointer(m_Window, &m_Data);
		// ������ֱͬ��
		SetVSync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});
	}

	void WindowsWindow::ShutDown() {
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled) {
		if (enabled) 
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const {
		return m_Data.VSync;
	}







}
