#pragma once
#include "AIEnviroment/Equipaments/Health/TypesHealth/NormalHealthEntity.h"
#include "AIEnviroment/Equipaments/Health/Mechanics/NormalHealth/FiniteStateHealthNormal.h"
namespace AIEnviroment {
	class WasDroppedNormal : public FiniteStateHealthNormal
	{
	private:
		WasDroppedNormal() {};
		static WasDroppedNormal* m_instance;
	public:
		static WasDroppedNormal* Instance();
		virtual void Enter(NormalHealthEntity* entity);
		virtual void Execute(NormalHealthEntity* entity);
		virtual void Exit(NormalHealthEntity* entity);
	};

}