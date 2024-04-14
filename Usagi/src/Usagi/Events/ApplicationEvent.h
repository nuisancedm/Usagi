#pragma once

#include "Event.h"

#include <sstream>

namespace Usagi {
	class USAGI_API WindwoResizeEvent : public Event {
	public:
		WindwoResizeEvent(unsigned int width, unsigned int height)
			:m_Width(width), m_Height(height) {}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		// for debug use
		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindwoResizeEvent: " << m_Width << " " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_Width, m_Height;
	};

	class USAGI_API WindowCloseEvent : public Event{
	public:
		WindowCloseEvent(){}
		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class USAGI_API AppTickEvent : public Event{
	public:
		AppTickEvent(){}
		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class USAGI_API AppUpdateEvent : public Event{
	public:
		AppUpdateEvent(){}
		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class USAGI_API AppRenderEvent : public Event{
	public:
		AppRenderEvent(){}
		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


}