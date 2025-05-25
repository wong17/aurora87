#pragma once
#include "AIEnviroment/DumbZombie/FiniteStateDumbZombie.h"
namespace AIEnviroment
{
	class ZombieDumbSleep :public FiniteStateDumbZombie
	{
	private:
		ZombieDumbSleep() {};
		static ZombieDumbSleep* m_instance;
	public:
		static ZombieDumbSleep* Instance();
		virtual void Enter(DumbZombie* entity);
		virtual void Execute(DumbZombie* entity);
		virtual void Exit(DumbZombie* entity);
	};

}