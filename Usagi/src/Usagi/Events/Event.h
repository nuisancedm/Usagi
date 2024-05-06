#pragma once

#include "Usagi/Core.h"
#include "usgpch.h"

namespace Usagi {


	enum class EventType 
	{
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
	enum EventCategory 
	{
		None = 0,
		EventCategoryApplication = BIT(0), // 00001
		EventCategoryInput       = BIT(1), // 00010
		EventCategoryKeyboard    = BIT(2), // 00100
		EventCategoryMouse       = BIT(3), // 01000
		EventCategoryMouseButton = BIT(4), // 10000
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; } \
								virtual EventType GetEventType() const override { return  GetStaticType(); } \
								virtual const char* GetName() const override {return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class USAGI_API Event 
	{
		friend class EventDispatcher;
	public:
		// if the event has been handled, we don't propgate the event any further
		bool Handled = false;
		virtual EventType GetEventType() const = 0;
		// get the string of the type
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		// check if the event is in the given category
		inline bool IsInCategory(EventCategory category) {
			return GetCategoryFlags() & category;
		}
	};

	class EventDispatcher 
	{
		// Template Aliases EventFn represents a function type that takes a reference to an object of type T and returns a boolean value.
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:

		// initializes the EventDispatcher with a reference to an Event object.
		EventDispatcher(Event& event)
			:m_Event(event) {}

		// Member function Dispatch()
		template<typename T>
		bool Dispatch(EventFn<T> func) {
			// Check if the m_Event type match the callback function type. it means that the event can be handled by the provided event handling function.
			if (m_Event.GetEventType() == T::GetStaticType()) {
				// pass the event to the callback function
				m_Event.Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;   
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<< (std::ostream& os, const Event& e) 
	{
		return os << e.ToString();
	}
}