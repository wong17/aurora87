#pragma once
class DumbZombie; 
class FiniteStateDumbZombie
{
public:
	virtual ~FiniteStateDumbZombie() {};

	virtual void Enter(DumbZombie*) = 0;
	virtual void Execute(DumbZombie*) = 0;
	virtual void Exit(DumbZombie*) = 0;
};

