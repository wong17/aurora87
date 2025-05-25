#pragma once
#include "AIEnviroment/FastZombie/FastStateFastZombie.h"
namespace AIEnviroment
{
	class ZombieFastSleep : public FiniteStateFastZombie
	{
	private:
		ZombieFastSleep() {};
		static ZombieFastSleep* m_instance;
	public:
		static ZombieFastSleep* Instance();
		virtual void Enter(FastZombie* entity);
		virtual void Execute(FastZombie* entity);
		virtual void Exit(FastZombie* entity);
	};
}
