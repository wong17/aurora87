#pragma once

#include "Game/AIEnviroment/FastZombie/Mechanics/SeekFast.h"
#include "Game/AIEnviroment/FastZombie/FastZombie.h"
#include "Game/AIEnviroment/FastZombie/FastStateFastZombie.h"

#include <glm/glm.hpp>

#include <iostream>

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
