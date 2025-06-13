#pragma once

#include "ChargeBoss.h"
#include "AttackDistanceBoss.h"

#include "Game/AIEnviroment/BossZombie/BossZombie.h"
#include <Game/AIEnviroment/Player/Player.h>

#include "Game/AIEnviroment/BossZombie/FiniteStateBossZombie.h"

#include <glm/glm.hpp>

namespace AIEnviroment 
{
	class SeekBoss : public FiniteStateBossZombie
	{

	private:
		SeekBoss() {};
		static SeekBoss* m_instance;

	public:
		static SeekBoss* Instance();
		virtual void Enter(BossZombie* entity);
		virtual void Execute(BossZombie* entity);
		virtual void Exit(BossZombie* entity);
	};
}
