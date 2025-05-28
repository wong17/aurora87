#pragma once
#include "AIEnviroment/Equipaments/Health/TypesHealth/LittleHealthEntity.h"
#include "AIEnviroment/Equipaments/Health/Mechanics/LittleHealth/FiniteStateHealthLittle.h"
namespace AIEnviroment {
	class WasDroppedLittle : public FiniteStateHealthLittle
	{
	private:
		WasDroppedLittle() {};
		static WasDroppedLittle* m_instance;
	public:
		static WasDroppedLittle* Instance();
		virtual void Enter(LittleHealthEntity* entity);
		virtual void Execute(LittleHealthEntity* entity);
		virtual void Exit(LittleHealthEntity* entity);
	};

}