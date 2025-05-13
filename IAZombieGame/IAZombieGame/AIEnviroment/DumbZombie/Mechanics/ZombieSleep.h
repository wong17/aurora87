#pragma once
#include "DumbZombie/FiniteStateDumbZombie.h"
class ZombieSleep : public FiniteStateDumbZombie
{
private:
	ZombieSleep() {};
	static ZombieSleep* m_instance;
public:
	static ZombieSleep* Instance();
	virtual void Enter(DumbZombie* entity);
	virtual void Execute(DumbZombie* entity);
	virtual void Exit(DumbZombie* entity);
};

