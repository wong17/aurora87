#pragma once
#include "AIEnviroment/Equipaments/Health/TypesHealth/BigHealthEntity.h"
#include "AIEnviroment/Equipaments/Health/Mechanics/BigHealth/FiniteStateHealthBig.h"
namespace AIEnviroment {
	class WasPicked : public FiniteStateHealthBig
	{
	private:
		WasPicked() {};
		static WasPicked* m_instance;
	public:
		static WasPicked* Instance();
		virtual void Enter(BigHealthEntity* entity);
		virtual void Execute(BigHealthEntity* entity);
		virtual void Exit(BigHealthEntity* entity);
	};
}
