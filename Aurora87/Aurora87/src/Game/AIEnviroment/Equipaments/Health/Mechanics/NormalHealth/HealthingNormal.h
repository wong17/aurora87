#pragma once
#include "AIEnviroment/Equipaments/Health/TypesHealth/NormalHealthEntity.h"
#include "AIEnviroment/Equipaments/Health/Mechanics/NormalHealth/FiniteStateHealthNormal.h"
namespace AIEnviroment {
	class HealthingNormal : public FiniteStateHealthNormal
	{
	private:
		HealthingNormal() {};
		static HealthingNormal* m_instance;
	public:
		static HealthingNormal* Instance();
		virtual void Enter(NormalHealthEntity* Entity);
		virtual void Execute(NormalHealthEntity* Entity);
		virtual void Exit(NormalHealthEntity* Entity);
	};

}