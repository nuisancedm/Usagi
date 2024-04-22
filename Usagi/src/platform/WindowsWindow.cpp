#include "usgpch.h"
#include "WindowsWindow.h"

#include "Usagi/Events/ApplicationEvent.h"
#include "Usagi/Events/KeyEvent.h"
#include "Usagi/Events/MouseEvent.h"


namespace Usagi {
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description) {
		USG_CORE_ERROR("GLFW Error ({0}):{1}", error, description);
	};

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

			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		// ����glfw����
		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		// ����������һ���û�ָ�룬���ָ��ָ��һƬ���ݣ�����Ƭ���ݹ������������
		glfwSetWindowUserPointer(m_Window, &m_Data);
		// ������ֱͬ��
		SetVSync(true);

		// ����GLFWWindowresize�ص����������������д�Ļص�������ֱ����lamdaд
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			// �����ð��ڵ�ǰ���ڵ��û�ָ��
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		// ���ڹر��¼�
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		// ���̰����¼�
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
				case GLFW_PRESS: {
					KeyPressedEvent event(key,0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT: {
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
			}
		});

		// ��갴���¼�
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int modes) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS: {
				MouseButtonPressedEvent event(button);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE: {
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
				break;
			}
			}
		});

		// �������¼�
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrollEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		// ����ƶ��¼�
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
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
