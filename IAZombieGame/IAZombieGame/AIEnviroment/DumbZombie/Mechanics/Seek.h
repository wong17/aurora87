
#include"FiniteStateDumbZombie.h"
#pragma once
class Seek : public FiniteStateDumbZombie
{

private:
	Seek() {};
	static Seek* m_instance;

public:
	static Seek* Instance();
	virtual void Enter(DumbZombie* entity);
	virtual void Execute(DumbZombie* entity);
	virtual void Exit(DumbZombie* entity);
};

