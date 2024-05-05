#pragma once

#include "Usagi/Window.h"
#include <GLFW/glfw3.h>

namespace Usagi {
	// WindowsWindow类继承自Window类，重写了Window类定义的虚函数，并进行了拓展，适用与Windows系统
	class WindowsWindow : public Window 
	{
	public:

		//构造和解构函数
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		// 重写Window类虚函数
		void OnUpdate() override;
		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		virtual void* GetNativeWindow() const { return m_Window; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

	private:

		// 这两个函数会在构造和解构函数中被调用
		virtual void Init(const WindowProps& props);
		virtual void ShutDown();

		// 使用GLFW窗口
		GLFWwindow* m_Window;
		// 内嵌WindowData结构体，存储了窗口相关的信息
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