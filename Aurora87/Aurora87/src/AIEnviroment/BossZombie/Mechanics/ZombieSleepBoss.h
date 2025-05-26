#pragma once
#include <AIEnviroment/BossZombie/FiniteStateBossZombie.h>

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