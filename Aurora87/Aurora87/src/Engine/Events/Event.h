#pragma once

#include "EventTypes.h"

#include <iostream>
#include <string>

namespace Engine
{
	inline const char* ExtractTypeName(const char* typeName)
	{
		const char* pos = strrchr(typeName, ':');
		return pos ? pos + 1 : typeName;
	}

#define EVENT_CLASS_TYPE(type) \
	static EventType GetStaticType() { return type; }\
	virtual EventType GetEventType() const override { return GetStaticType(); }\
	virtual const char* GetName() const override { return Engine::ExtractTypeName(#type); }

#define EVENT_CLASS_CATEGORY(category) \
	virtual int GetCategoryFlags() const override { return static_cast<int>(category); }

	class Event
	{
	public:
		bool Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual std::string ToString() const { return GetName(); }
		virtual int GetCategoryFlags() const = 0;
		inline bool IsInCategory(EventCategory category) const
		{
			return GetCategoryFlags() & static_cast<int>(category);
		}
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		template<typename T, typename F>
		bool Dispatch(const F& funct)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = funct(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};
}