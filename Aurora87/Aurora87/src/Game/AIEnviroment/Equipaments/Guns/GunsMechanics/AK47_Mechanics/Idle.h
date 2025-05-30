#pragma once
#include "AIEnviroment/EventGame/EventData.h"
#include <AIEnviroment/EventGame/EventManager.h>
#include <AIEnviroment/Equipaments/Guns/GunsMechanics/FiniteStateAK47.h>
namespace AIEnviroment {
	class Idle : public FiniteStateAK7, public EventListener
	{
	private:
		Idle() {};
		static Idle* m_instance;
	public:
		static Idle* Instance();
		virtual void Enter(AK47_Entity* entity);
		virtual void Execute(AK47_Entity* entity);
		virtual void Exit(AK47_Entity* entity);
		void HandleEvent(const EventData& eventData) override;

	};

}
