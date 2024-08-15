#include "usgpch.h"
#include "WindowsWindow.h"

#include "Usagi/Events/ApplicationEvent.h"
#include "Usagi/Events/KeyEvent.h"
#include "Usagi/Events/MouseEvent.h"

#include "platform/OpenGL/OpenGLContext.h"



namespace Usagi {
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description) {
		USG_CORE_ERROR("GLFW Error ({0}):{1}", error, description);
	};

	// static funciton "Window* Window::Create"  WindowsWindow implementation 
	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	// constructor
	WindowsWindow::WindowsWindow(const WindowProps& props) {
		Init(props);
	}

	// destructor 
	WindowsWindow::~WindowsWindow() {
		ShutDown();
	}

	void WindowsWindow::Init(const WindowProps& props) {
		
		m_Data.Title = props.Title;										//@@ use WIndowProps to init m_Data
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		USG_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);
			
		if (!s_GLFWInitialized) {										//@@ check if glfw is inited.									
			int success = glfwInit();
			USG_CORE_ASSERT(success, "Could not initialize GLFW!");

			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);   //@@ create glfw Window
		
		m_Context = new OpenGLContext(m_Window);                        //@@ create Opengl Context
		m_Context->Init();

		
		// ========= OPENGL NOTES ==========
		// m_Data is a member of the WindowsWindow, in normal case glfwWindow can't access this data.
		// glfwSetWindowUserPointer() bind a pointer to ht glfwWindow, so it can be access by glfwwindow in the callback function.
		// ================================
		glfwSetWindowUserPointer(m_Window, &m_Data);                    //@@ set glfwWindows's user data pointer
		SetVSync(true);

		
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) { //@@ set callbacks, use lamda here.
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);              //@@ use glfwGetWindowUserPointer to get the data bind to the glfwWindow.
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

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

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

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
		m_Context->SwapBuffers();
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
