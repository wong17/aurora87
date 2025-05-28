#pragma once
#include "AIEnviroment/Player/FiniteStatePlayer.h"
#include "AIEnviroment/EventGame/EventManager.h"
#include "AIEnviroment/EventGame/EventData.h"
#include "AIEnviroment/Player/Player.h"
namespace AIEnviroment
{
	class Walk : public FiniteStatePlayer, public EventListener
	{
	private:
		static Walk* m_instance;

	public:
		Walk();
		~Walk();
		static Walk* Instance();
		virtual void Enter(Player* entity);
		virtual void Execute(Player* entity);
		virtual void Exit(Player* entity);

		void HandleEvent(const EventData& event);
	};

}