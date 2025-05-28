#pragma once
#include "AIEnviroment/FastZombie/FastStateFastZombie.h"

namespace AIEnviroment
{
	class SeekFast : public FiniteStateFastZombie
	{
	private:
		SeekFast() {};
		static SeekFast* m_instance;

	public:
		static SeekFast* Instance();
		virtual void Enter(FastZombie* entity);
		virtual void Execute(FastZombie* entity);
		virtual void Exit(FastZombie* entity);
	};

}