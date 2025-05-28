#pragma once
namespace AIEnviroment {
	class BigHealthEntity;
	class FiniteStateHealthBig {
		public:
			virtual ~FiniteStateHealthBig() {};
			virtual void Enter(BigHealthEntity*) = 0;
			virtual void Execute(BigHealthEntity*) = 0;
			virtual void Exit(BigHealthEntity*) = 0;
		};
}