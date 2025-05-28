#pragma once
#include "AIEnviroment/Equipaments/Health/TypesHealth/LittleHealthEntity.h"
#include "AIEnviroment/Equipaments/Health/Mechanics/LittleHealth/FiniteStateHealthLittle.h"
namespace AIEnviroment {
	class HealthingLittle : public FiniteStateHealthLittle
	{
	private:
		HealthingLittle() {};
		static HealthingLittle* m_instance;
	public:
		static HealthingLittle* Instance();
		virtual void Enter(LittleHealthEntity* Entity);
		virtual void Execute(LittleHealthEntity* Entity);
		virtual void Exit(LittleHealthEntity* Entity);
	};

}