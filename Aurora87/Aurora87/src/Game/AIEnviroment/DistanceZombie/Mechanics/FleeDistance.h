#pragma once

#include "SeekDistance.h"

#include "Game/AIEnviroment/DistanceZombie/DistanceZombie.h"
#include "Game/AIEnviroment/DistanceZombie/FiniteStateDistanceZombie.h"

#include "Game/AIEnviroment/Player/Player.h"

#include <glm/glm.hpp>

#include <iostream>

namespace AIEnviroment 
{
	class FleeDistance : public FiniteStateDistanceZombie
	{
	private:
		FleeDistance() {};
		static FleeDistance* m_instance;
	public:
		static FleeDistance* Instance();
		virtual void Enter(DistanceZombie* entity);
		virtual void Execute(DistanceZombie* entity);
		virtual void Exit(DistanceZombie* entity);
	};

}