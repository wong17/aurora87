#pragma once

#include"EventData.h"

#include<vector>
#include<map>

namespace AIEnviroment 
{

	class EventListener 
	{
	public:
		virtual ~EventListener() {};
		virtual void HandleEvent(const EventData& eventData) = 0;
	};

	class EventManager
	{
	private:
		static EventManager* instance;
		std::map<EventType, std::vector<EventListener*>> listeners;
		EventManager() = default;
		EventManager(const EventManager&) = delete;
		EventManager& operator=(const EventManager&) = delete;

	public:
		static EventManager* GetInstance();

		void AddListener(EventType eventType, EventListener* listener);
		void RemoveListener(EventType eventType, EventListener* listener);
		void DispatchEvent(const EventData& eventData);
	};

}