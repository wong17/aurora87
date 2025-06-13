#pragma once

#include "SeekBoss.h"

#include "Game/AIEnviroment/BossZombie/FiniteStateBossZombie.h"
#include "Game/AIEnviroment/BossZombie/BossZombie.h"

#include <glm/glm.hpp>

#include <iostream>

namespace AIEnviroment
{
	class ZombieSleepBoss : public FiniteStateBossZombie
	{
	private:
		ZombieSleepBoss() {};
		static ZombieSleepBoss* m_instance;
	public:
		static ZombieSleepBoss* Instance();
		virtual void Enter(BossZombie* entity);
		virtual void Execute(BossZombie* entity);
		virtual void Exit(BossZombie* entity);
	};

}