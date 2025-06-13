#pragma once

namespace AIEnviroment
{
	// Forward declaration of BossZombie class
	class BossZombie;
	class FiniteStateBossZombie 
	{
	public:
		virtual ~FiniteStateBossZombie() {};
		virtual void Enter(BossZombie*) = 0;
		virtual void Execute(BossZombie*) = 0;
		virtual void Exit(BossZombie*) = 0;
	};
}