#pragma once
#include "Usagi/Renderer/GraphicContext.h"

struct GLFWwindow;

namespace Usagi
{
	class OpenGLContext : public GraphicContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}