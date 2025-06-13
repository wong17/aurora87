#pragma once

namespace AIEnviroment
{
	class Player;

	class FiniteStatePlayer
	{
	public:
		virtual ~FiniteStatePlayer() {};
		virtual void Enter(Player*) = 0;
		virtual void Execute(Player*) = 0;
		virtual void Exit(Player*) = 0;
	};
}
