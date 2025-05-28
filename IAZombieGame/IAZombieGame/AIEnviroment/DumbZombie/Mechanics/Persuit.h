#include "FiniteStateDumbZombie.h"

#pragma once
class Persuit : public FiniteStateDumbZombie
{
private:
	 Persuit() {};

public:
	virtual void Enter(DumbZombie* entity);
	virtual void Execute(DumbZombie* entity);
	virtual void Exit(DumbZombie* entity);
};

