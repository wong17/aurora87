#pragma once

namespace AIEnviroment
{
	// Forward declaration of DistanceZombie class
	class DistanceZombie; 
	class FiniteStateDistanceZombie
	{

	public:
		virtual ~FiniteStateDistanceZombie() {};

		virtual void Enter(DistanceZombie*) = 0;
		virtual void Execute(DistanceZombie*) = 0;
		virtual void Exit(DistanceZombie*) = 0;
	};
}