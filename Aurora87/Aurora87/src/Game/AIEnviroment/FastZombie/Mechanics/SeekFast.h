#pragma once

#include "Game/AIEnviroment/Player/Player.h"

#include "Game/AIEnviroment/FastZombie/FastZombie.h"
#include "Game/AIEnviroment/FastZombie/FastStateFastZombie.h"

#include <glm/glm.hpp>

#include <iostream>

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