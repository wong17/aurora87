#pragma once
#include "AIEnviroment/Equipaments/Health/TypesHealth/BigHealthEntity.h"
#include "AIEnviroment/Equipaments/Health/Mechanics/BigHealth/FiniteStateHealthBig.h"
namespace AIEnviroment {
	class HealthingBig : public FiniteStateHealthBig
	{
	private: 
		HealthingBig(){};
		static HealthingBig* m_instance;
	public :
		static HealthingBig* Instance();
		virtual void Enter(BigHealthEntity* Entity);
		virtual void Execute(BigHealthEntity* Entity);
		virtual void Exit(BigHealthEntity* Entity);
	};

}