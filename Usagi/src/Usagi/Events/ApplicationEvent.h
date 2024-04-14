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

	// TODO
}