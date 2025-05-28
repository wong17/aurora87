#pragma once
#include "AIEnviroment/Equipaments/Guns/GunsMechanics/FiniteStateAK47.h"
namespace AIEnviroment {
	class Load : public FiniteStateAK7
	{
	private:
		Load() {};
		static Load* m_instance;

	public:
		static Load* Instance();
		virtual void Enter(AK47_Entity* entity);
		virtual void Execute(AK47_Entity* entity);
		virtual void Exit(AK47_Entity* entity);

	};
}
