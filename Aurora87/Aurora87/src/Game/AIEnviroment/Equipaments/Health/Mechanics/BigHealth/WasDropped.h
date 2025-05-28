#pragma once
#include "AIEnviroment/Equipaments/Health/TypesHealth/BigHealthEntity.h"
#include "AIEnviroment/Equipaments/Health/Mechanics/BigHealth/FiniteStateHealthBig.h"
namespace AIEnviroment {
	class WasDropped : public FiniteStateHealthBig
	{
	private:
		WasDropped() {};
		static WasDropped* m_instance;
	public:
		static WasDropped* Instance();
		virtual void Enter(BigHealthEntity* entity);
		virtual void Execute(BigHealthEntity* entity);
		virtual void Exit(BigHealthEntity* entity);
	};

}