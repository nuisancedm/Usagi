#pragma once

#include "usgpch.h"
#include "Usagi/Core.h"
#include "Usagi/Events/Event.h"

namespace Usagi {
	// Window���Խṹ�壬�����˳�����⣬��Ĭ��ֵ
	struct WindowProps {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Usagi", unsigned int width = 1280, unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{}
	};

	// һ������Window���࣬���ݲ�ͬ�Ĳ���ϵͳʵ������ͬ������
	class USAGI_API Window {
	public:
		// �¼��ص�����ָ��
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		// ������ÿ�����������е��ã����д�����صĸ��¡�
		virtual void OnUpdate() = 0;
		
		//���ش��ڵĳߴ硣
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// �����¼���������
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		// ���û���ô�ֱͬ����
		virtual void SetVSync(bool enabled) = 0;
		// ���ش�ֱͬ��������״̬��
		virtual bool IsVSync() const = 0;

		// ��̬ �������ں���
		static Window* Create(const WindowProps& props = WindowProps());
	};
}