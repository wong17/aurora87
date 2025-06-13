#pragma once

namespace AIEnviroment 
{
	class AK47_Entity;
	class FiniteStateAK7 
	{
	public:
		virtual ~FiniteStateAK7() {};
		virtual void Enter(AK47_Entity*) = 0;
		virtual void Execute(AK47_Entity*) = 0;
		virtual void Exit(AK47_Entity*) = 0;
	};
}
