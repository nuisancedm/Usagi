#pragma once

#include "Usagi/Window.h"
#include <GLFW/glfw3.h>

namespace Usagi {
	// WindowsWindow��̳���Window�࣬��д��Window�ඨ����麯��������������չ��������Windowsϵͳ
	class WindowsWindow : public Window 
	{
	public:

		//����ͽ⹹����
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		// ��дWindow���麯��
		void OnUpdate() override;
		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		virtual void* GetNativeWindow() const { return m_Window; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

	private:

		// �������������ڹ���ͽ⹹�����б�����
		virtual void Init(const WindowProps& props);
		virtual void ShutDown();

		// ʹ��GLFW����
		GLFWwindow* m_Window;
		// ��ǶWindowData�ṹ�壬�洢�˴�����ص���Ϣ
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