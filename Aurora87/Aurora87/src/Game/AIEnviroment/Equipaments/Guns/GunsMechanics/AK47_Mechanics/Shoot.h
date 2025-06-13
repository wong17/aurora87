#pragma once

#include "Game/AIEnviroment/Equipaments/Guns/GunsMechanics/FiniteStateAK47.h"

namespace AIEnviroment 
{
	class Shoot : public FiniteStateAK7
	{
	private:
		Shoot() {};
		static Shoot* m_instance;
	public:
		static Shoot* Instance();
		virtual void Enter(AK47_Entity* entity);
		virtual void Execute(AK47_Entity* entity);
		virtual void Exit(AK47_Entity* entity);
	};

}