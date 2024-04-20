#pragma once

#include "usgpch.h"
#include "Usagi/Core.h"
#include "Usagi/Events/Event.h"

namespace Usagi {
	// Window属性结构体，包含了长宽标题，有默认值
	struct WindowProps {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Usagi", unsigned int width = 1280, unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{}
	};

	// 一个纯虚Window基类，根据不同的操作系统实例化不同的子类
	class USAGI_API Window {
	public:
		// 事件回调函数指针
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		// 用于在每个更新周期中调用，进行窗口相关的更新。
		virtual void OnUpdate() = 0;
		
		//返回窗口的尺寸。
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// 设置事件处理函数。
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		// 启用或禁用垂直同步。
		virtual void SetVSync(bool enabled) = 0;
		// 返回垂直同步的启用状态。
		virtual bool IsVSync() const = 0;

		// 静态 创建窗口函数
		static Window* Create(const WindowProps& props = WindowProps());
	};
}