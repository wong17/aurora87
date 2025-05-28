#include "AttackHandsToHands.h"
#include "Player.h"
#include "Walk.h"
AttackHandsToHands::AttackHandsToHands()
{
	EventManager::GetInstance()->AddListener(EventType::PLAYER_ATTACK, this);
}
AttackHandsToHands::~AttackHandsToHands()
{
	EventManager::GetInstance()->RemoveListener(EventType::PLAYER_ATTACK, this);

}
AttackHandsToHands* AttackHandsToHands::Instance()
{
	if (!m_instance)
	{
		m_instance = new AttackHandsToHands();
	}
	return m_instance;
}

void AttackHandsToHands::Enter(Player* entity)
{
	entity->ClosestZombieTarget();
	entity->SetAttackDamageRange(6.0f);

}

void AttackHandsToHands::Execute(Player* entity)
{
	entity->ClosestZombieTarget();
	
}

void AttackHandsToHands::Exit(Player* entity)
{
}

void AttackHandsToHands::HandleEvent(const EventData& event)
{
	if (event.type == EventType::PLAYER_ATTACK) {
		BaseGameEntity* entityBase = EntityManager::Instance().GetEntity(event.GetInt("playerIDAttack"));
		Player* entity = dynamic_cast<Player*>(entityBase);
		entity->SetAttackDamage(100.0f);

		float distanceToZombie = entity->GetDistanceToZombie();
		if (distanceToZombie <= entity->GetAttackRange()) {
			entity->SetAttackCooldonwTimer(entity->GetAttackCooldownTimer() + entity->GetDeltaTime());
			EventData playerPunchEvent(EventType::PLAYER_PUNCH);
			playerPunchEvent.Set("PlayerPunchDamage", entity->GetAttackDamage());
			if (entity->GetAttackCooldownTimer() >= entity->GetAttackCooldown()) {
				playerPunchEvent.Set("ZombiePunchID", entity->GetZombieTargetID());
				playerPunchEvent.Set("PlayerPunchID", entity->GetID());
				std::cout << "In range" << std::endl;
				EventManager::GetInstance()->DispatchEvent(playerPunchEvent);
				entity->SetAttackCooldonwTimer(0.0f);
			}
		}
		else
		{
			entity->ChangeFiniteStatePlayer(Walk::Instance());
		}
	}
}
AttackHandsToHands* AttackHandsToHands::m_instance = nullptr; // Initialize the static instance pointer