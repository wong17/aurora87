#pragma once

#include "Game/AIEnviroment/DumbZombie/DumbZombie.h"
#include "Game/AIEnviroment/DumbZombie/Mechanics/Seek.h"
#include "Game/AIEnviroment/DumbZombie/FiniteStateDumbZombie.h"

#include <glm/glm.hpp>

#include <iostream>

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