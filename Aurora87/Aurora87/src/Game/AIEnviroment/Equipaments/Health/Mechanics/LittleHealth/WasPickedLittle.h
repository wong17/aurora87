#pragma once
#include "AIEnviroment/Equipaments/Health/TypesHealth/LittleHealthEntity.h"
#include "AIEnviroment/Equipaments/Health/Mechanics/LittleHealth/FiniteStateHealthLittle.h"
namespace AIEnviroment {
	class WasPickedLittle : public FiniteStateHealthLittle
	{
	private:
		WasPickedLittle() {};
		static WasPickedLittle* m_instance;
	public:
		static WasPickedLittle* Instance();
		virtual void Enter(LittleHealthEntity* entity);
		virtual void Execute(LittleHealthEntity* entity);
		virtual void Exit(LittleHealthEntity* entity);
	};
}
