#pragma once

#include "Usagi/Core.h"

#include <string>
#include <functional>

namespace Usagi {
	// events is Usagi are currintlt blocking , meaning when an event occurs it
	// immediatelt gets dispatched and must be dealt with right then an there.
	// for the future, a better strategy might be to buffer events in an event
	// bus and process them during the "event" part of thr update stage.

	enum class EventType {
		None = 0,
		// Window event
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		// Application event
		AppTick, AppUpdate, AppRender,
		// Key Event
		KeyPressed, KeyReleased,
		// Mouse Event
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	// EventCategory is used to filter certain event
	enum EventCategory {
		None = 0,
		EventCategoryApplication = BIT(0), // 00001
		EventCategoryInput       = BIT(1), // 00010
		EventCategoryKeyboard    = BIT(2), // 00100
		EventCategoryMouse       = BIT(3), // 01000
		EventCategoryMouseButton = BIT(4), // 10000
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; } \
								virtual EventType GetEventType() const override { return  GetStaticType(); } \
								virtual const char* GetName() const override {return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class USAGI_API Event {
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category) {
			return GetCategoryFlags() & category;
		}
	protected:
		// if the event has been handled, we don't propgate the event any further
		bool m_Handled = false;
	};

	class EventDispatcher {
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			:m_Event(event) {}

		template<typename T>
		bool Dispatch(EventFn<T> func) {
			if (m_Event.GetEventType() == T::GetStaticType()) {
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}
}