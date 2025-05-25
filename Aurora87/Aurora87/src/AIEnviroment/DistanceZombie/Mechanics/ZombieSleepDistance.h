#pragma once
#include <AIEnviroment/DistanceZombie/FiniteStateDistanceZombie.h>
namespace AIEnviroment {
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