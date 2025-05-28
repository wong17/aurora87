#pragma once
#include "AIEnviroment/Equipaments/Health/TypesHealth/NormalHealthEntity.h"
#include "AIEnviroment/Equipaments/Health/Mechanics/NormalHealth/FiniteStateHealthNormal.h"
namespace AIEnviroment {
	class WasPickedNormal : public FiniteStateHealthNormal
	{
	private:
		WasPickedNormal() {};
		static WasPickedNormal* m_instance;
	public:
		static WasPickedNormal* Instance();
		virtual void Enter(NormalHealthEntity* entity);
		virtual void Execute(NormalHealthEntity* entity);
		virtual void Exit(NormalHealthEntity* entity);
	};
}
