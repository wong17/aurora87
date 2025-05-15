#pragma once

#include "Event.h"
#include "EventTypes.h"

#include <string>
#include <sstream>

namespace Engine
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height)
		{
		}
		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}
		EVENT_CLASS_TYPE(EventType::WindowResize)
		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryWindow)
	private:
		unsigned int m_Width, m_Height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;
		std::string ToString() const override
		{
			return "WindowCloseEvent";
		}
		EVENT_CLASS_TYPE(EventType::WindowClose)
		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryWindow)
	};
}