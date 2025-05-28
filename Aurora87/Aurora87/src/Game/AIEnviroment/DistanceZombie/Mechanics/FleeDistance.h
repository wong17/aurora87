#pragma once
#include <AIEnviroment/DistanceZombie/FiniteStateDistanceZombie.h>

namespace AIEnviroment {
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