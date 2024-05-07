#pragma once

#include "usgpch.h"
#include "Usagi/Core.h"
#include "Usagi/Events/Event.h"

namespace Usagi {

	struct WindowProps {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		// default value
		WindowProps(const std::string& title = "Usagi", unsigned int width = 1280, unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{}
	};

	// pure virtual Window interface
	class USAGI_API Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;
		
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// 
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}