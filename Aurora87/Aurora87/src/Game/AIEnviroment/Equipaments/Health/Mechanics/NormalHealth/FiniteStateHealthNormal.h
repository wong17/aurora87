#pragma once
namespace AIEnviroment {
	class NormalHealthEntity;
	class FiniteStateHealthNormal {
	public:
		virtual ~FiniteStateHealthNormal() {};
		virtual void Enter(NormalHealthEntity*) = 0;
		virtual void Execute(NormalHealthEntity*) = 0;
		virtual void Exit(NormalHealthEntity*) = 0;
	};
}