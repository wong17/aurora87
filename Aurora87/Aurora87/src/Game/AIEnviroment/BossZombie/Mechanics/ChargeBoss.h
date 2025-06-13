#pragma once

#include "SeekBoss.h"

#include "Game/AIEnviroment/BossZombie/FiniteStateBossZombie.h"
#include "Game/AIEnviroment/BossZombie/BossZombie.h"

#include "Game/AIEnviroment/Player/Player.h"

#include "Game/AIEnviroment/Base/GameEntityManager.h"

#include <glm/glm.hpp>

#include <iostream>

namespace AIEnviroment 
{
	class ChargeBoss : public FiniteStateBossZombie
	{
	private:
		ChargeBoss() {};
		static ChargeBoss* m_instance;
	public:
		static ChargeBoss* Instance();
		virtual void Enter(BossZombie* entity);
		virtual void Execute(BossZombie* entity);
		virtual void Exit(BossZombie* entity);

	};
}