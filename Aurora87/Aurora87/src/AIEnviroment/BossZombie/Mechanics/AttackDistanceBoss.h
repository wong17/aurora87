#pragma once
#include <AIEnviroment/BossZombie/FiniteStateBossZombie.h>

namespace AIEnviroment {
	class AttackDistanceBoss : public FiniteStateBossZombie
	{
	private:
		AttackDistanceBoss() {};
		static AttackDistanceBoss* m_instance;
	public:
		static AttackDistanceBoss* Instance();
		virtual void Enter(BossZombie* entity);
		virtual void Execute(BossZombie* entity);
		virtual void Exit(BossZombie* entity);
	};

}