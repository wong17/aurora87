#include "FiniteStatePlayer.h"
#include"Camera.h"

#pragma once
class Walk : public FiniteStatePlayer, public EventListener
{
private:
	static Walk* m_instance;

public:
	Walk();
	~Walk();
	static Walk* Instance();
	virtual void Enter(Player* entity);
	virtual void Execute(Player* entity);
	virtual void Exit(Player* entity);

	void HandleEvent(const EventData& event);
};

