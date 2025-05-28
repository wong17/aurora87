#pragma once
#include <AIEnviroment/BossZombie/FiniteStateBossZombie.h>
namespace AIEnviroment {
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