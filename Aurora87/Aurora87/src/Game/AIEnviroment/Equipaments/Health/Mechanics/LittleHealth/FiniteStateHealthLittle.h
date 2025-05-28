#pragma once
namespace AIEnviroment {
	class LittleHealthEntity;
	class FiniteStateHealthLittle {
	public:
		virtual ~FiniteStateHealthLittle() {};
		virtual void Enter(LittleHealthEntity*) = 0;
		virtual void Execute(LittleHealthEntity*) = 0;
		virtual void Exit(LittleHealthEntity*) = 0;
	};
}