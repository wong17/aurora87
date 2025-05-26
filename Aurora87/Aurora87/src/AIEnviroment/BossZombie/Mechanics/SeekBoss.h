#pragma once
#include <AIEnviroment/BossZombie/FiniteStateBossZombie.h>

namespace AIEnviroment {
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
