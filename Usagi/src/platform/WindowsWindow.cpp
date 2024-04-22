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

	// Window* Window::Create实现，返回一个WindowsWindow类实例的地址
	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	// WindowsWindow 构造函数，调用WindowsWindow的私有方法Init
	WindowsWindow::WindowsWindow(const WindowProps& props) {
		Init(props);
	}

	// WindowsWindow 解构函数，调用WindowsWindow的私有方法ShutDown
	WindowsWindow::~WindowsWindow() {
		ShutDown();
	}

	// WindowsWindow私有方法Init的具体实现
	void WindowsWindow::Init(const WindowProps& props) {
		// m_Data初始化
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		// core info
		USG_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);


		if (!s_GLFWInitialized) {
			// 调用glfwInit()，返回一个状态码，代表glfw初始化是否成功
			int success = glfwInit();
			// 此行生效的前提是开启了ASSERT
			USG_CORE_ASSERT(success, "Could not initialize GLFW!");

			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		// 创建glfw窗口
		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		// 给窗口设置一个用户指针，这个指针指向一片数据，将这片数据关联到这个窗口
		glfwSetWindowUserPointer(m_Window, &m_Data);
		// 开启垂直同步
		SetVSync(true);

		// 设置GLFWWindowresize回调，在这里调用我们写的回调函数，直接用lamda写
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			// 解引用绑定在当前窗口的用户指针
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		// 窗口关闭事件
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		// 键盘按键事件
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

		// 鼠标按键事件
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

		// 鼠标滚轮事件
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrollEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		// 鼠标移动事件
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
