#pragma once

#include "SeekDistance.h"

#include "Game/AIEnviroment/DistanceZombie/DistanceZombie.h"
#include "Game/AIEnviroment/DistanceZombie/FiniteStateDistanceZombie.h"

#include <glm/glm.hpp>

#include <iostream>

namespace AIEnviroment 
{
	class ZombieSleepDistance : public FiniteStateDistanceZombie
	{
	private:
		ZombieSleepDistance() {};
		static ZombieSleepDistance* m_instance;
	public:
		static ZombieSleepDistance* Instance();
		virtual void Enter(DistanceZombie* entity);
		virtual void Execute(DistanceZombie* entity);
		virtual void Exit(DistanceZombie* entity);
	};

}