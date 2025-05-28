#pragma once
#include <AIEnviroment/DistanceZombie/FiniteStateDistanceZombie.h>
namespace AIEnviroment {
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