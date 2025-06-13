#pragma once

#include "Game/AIEnviroment/Player/FiniteStatePlayer.h"
#include "Game/AIEnviroment/Player/Player.h"

#include "Game/AIEnviroment/EventGame/EventManager.h"
#include "Game/AIEnviroment/EventGame/EventData.h"

#include <glm/glm.hpp>

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