#include "usgpch.h"
#include "OpenGLContext.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>


namespace Usagi 
{
	

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) 
		:m_WindowHandle(windowHandle)
	{
		USG_CORE_ASSERT(windowHandle, "windowHandle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		USG_CORE_ASSERT(status, "Failed to initialize Glad!");

		// --- device info ---
		USG_CORE_INFO("OpenGL Info:");
		USG_CORE_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		USG_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		USG_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));


	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}