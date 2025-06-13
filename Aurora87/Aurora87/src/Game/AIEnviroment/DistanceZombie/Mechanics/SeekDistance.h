#pragma once

#include "FleeDistance.h"

#include "Game/AIEnviroment/DistanceZombie/DistanceZombie.h"
#include "Game/AIEnviroment/DistanceZombie/FiniteStateDistanceZombie.h"

#include <glm/glm.hpp>

#include <iostream>

namespace AIEnviroment 
{
	class SeekDistance : public FiniteStateDistanceZombie
	{
	private:
		SeekDistance() {};
		static SeekDistance* m_instance;
	public:
		static SeekDistance* Instance();
		virtual void Enter(DistanceZombie* entity);
		virtual void Execute(DistanceZombie* entity);
		virtual void Exit(DistanceZombie* entity);
	};

}