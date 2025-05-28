#pragma once
#include <AIEnviroment/Equipaments/Guns/GunsMechanics/FiniteStateAK47.h>
namespace AIEnviroment {
	class Idle : public FiniteStateAK7
	{
	private:
		Idle() {};
		static Idle* m_instance;
	public:
		static Idle* Instance();
		virtual void Enter(AK47_Entity* entity);
		virtual void Execute(AK47_Entity* entity);
		virtual void Exit(AK47_Entity* entity);
	};

}
