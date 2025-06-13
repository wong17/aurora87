#pragma once

#include "Game/AIEnviroment/DumbZombie/DumbZombie.h"
#include "Game/AIEnviroment/DumbZombie/FiniteStateDumbZombie.h"

#include "Game/AIEnviroment/EventGame/EventManager.h"
#include "Game/AIEnviroment/EventGame/EventData.h"

#include "Game/AIEnviroment/Player/Player.h"

#include <glm/glm.hpp>

#include <iostream>

namespace AIEnviroment
{
	class Seek : public FiniteStateDumbZombie
	{

	private:
		Seek() {};
		static Seek* m_instance;

	public:
		static Seek* Instance();
		virtual void Enter(DumbZombie* entity);
		virtual void Execute(DumbZombie* entity);
		virtual void Exit(DumbZombie* entity);
	};
}
