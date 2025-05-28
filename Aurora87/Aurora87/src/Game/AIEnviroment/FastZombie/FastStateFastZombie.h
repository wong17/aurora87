#pragma once

namespace AIEnviroment
{
	class FastZombie;
	class FiniteStateFastZombie {
	public:
		virtual ~FiniteStateFastZombie() {};

		virtual void Enter(FastZombie*) = 0;
		virtual void Execute(FastZombie*) = 0;
		virtual void Exit(FastZombie*) = 0;
	};
}