#include "EventManager.h"

EventManager* EventManager::instance = nullptr;
EventManager* EventManager::GetInstance()
{
	if (!instance)
	{
		instance = new EventManager();
	}
	return instance;
}

void EventManager::AddListener(EventType eventType, EventListener* listener)
{
	listeners[eventType].push_back(listener); 
}

void EventManager::RemoveListener(EventType eventType, EventListener* listener)
{
	auto it = listeners.find(eventType);
	if (it != listeners.end())
	{
		auto& listenerList = it->second;
		listenerList.erase(std::remove(listenerList.begin(), listenerList.end(), listener), listenerList.end());
	}
}

void EventManager::DispatchEvent(const EventData& eventData)
{
	auto it = listeners.find(eventData.type);
	if (it != listeners.end())
	{
		for (auto& listener : it->second)
		{
			listener->HandleEvent(eventData);
		}
	}
}
