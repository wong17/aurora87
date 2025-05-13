#pragma once
#include "FiniteStatePlayer.h"
#include "EventManager.h"
#include "EventData.h"
class AttackHandsToHands : public FiniteStatePlayer, public EventListener
{

private:
	static AttackHandsToHands* m_instance;

public:
	AttackHandsToHands();
	~AttackHandsToHands();
	static AttackHandsToHands* Instance();
	virtual void Enter(Player* entity);
	virtual void Execute(Player* entity);
	virtual void Exit(Player* entity);
	void HandleEvent(const EventData& event);
};

