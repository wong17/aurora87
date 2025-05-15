#pragma once

#include <cstdint>

namespace Engine
{
	enum class EventType
	{
		None = 0,

		WindowClose, 
		WindowResize, 
		WindowFocus, 
		WindowLostFocus, 
		WindowMoved,
		
		AppTick, 
		AppUpdate, 
		AppRender,

		KeyPressed, 
		KeyReleased, 
		KeyTyped,

		MouseButtonPressed, 
		MouseButtonReleased, 
		MouseMoved, 
		MouseScrolled
	};

	enum class EventCategory : uint32_t
	{
		None						= 0,
		EventCategoryApplication	= 1 << 0,
		EventCategoryInput			= 1 << 1,
		EventCategoryKeyboard		= 1 << 2,
		EventCategoryMouse			= 1 << 3,
		EventCategoryMouseButton	= 1 << 4,
		EventCategoryWindow			= 1 << 5
	};

	inline EventCategory operator|(EventCategory a, EventCategory b) 
	{ 
		return static_cast<EventCategory>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b)); 
	}
}