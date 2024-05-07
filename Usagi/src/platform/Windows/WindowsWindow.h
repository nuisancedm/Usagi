#pragma once

#include "Usagi/Window.h"
#include <GLFW/glfw3.h>

namespace Usagi {

	class WindowsWindow : public Window 
	{
	public:

		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		// overriding base class's interface
		void OnUpdate() override;
		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		virtual void* GetNativeWindow() const { return m_Window; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

	private:

		virtual void Init(const WindowProps& props);
		virtual void ShutDown();

		// windows window using glfw window
		GLFWwindow* m_Window;
		
		struct WindowData 
		{
			std::string Title;
			unsigned int Width;
			unsigned int Height;
			bool VSync;
			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}