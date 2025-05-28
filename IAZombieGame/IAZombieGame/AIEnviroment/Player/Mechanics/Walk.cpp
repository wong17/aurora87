#include "Walk.h"
#include "Player.h"
#include "EventManager.h"
#include "EventData.h"
#include "AttackHandsToHands.h"
#include <Base/EntityManager.h>
Walk::Walk()
{
	EventManager::GetInstance()->AddListener(EventType::PLAYER_MOVE, this);

}Walk::~Walk()
{
	EventManager::GetInstance()->RemoveListener(EventType::PLAYER_MOVE, this);

}
Walk* Walk::Instance()
{
	if (!m_instance)
	{
		m_instance = new Walk;
	}
	return m_instance;
}
void Walk::Enter(Player* entity)
{
	entity->SetSpeed(0.1f);
	entity->ClosestZombieTarget();
}

void Walk::Execute(Player* entity)
{
	entity->ClosestZombieTarget();
	glm::vec3 targetPosition = entity->GetTargetPosition();
	glm::vec3 currentPosition = entity->GetPosition();

    entity->SetDistanceToZombie( glm::length(targetPosition - currentPosition));
	if (entity->GetDistanceToZombie() <= entity->GetAttackRange()) {
		entity->ChangeFiniteStatePlayer(AttackHandsToHands::Instance());
	}
}

void Walk::Exit(Player* entity)
{
}
void Walk::HandleEvent(const EventData& event)
{
	if (event.type == EventType::PLAYER_MOVE)
	{
		glm::vec3 newPosition = {
			event.GetFloat("newPositionX"),
			event.GetFloat("newPositiony"),
			event.GetFloat("newPositionz")
		};
		int playerID = event.GetInt("playerMovedID");
		BaseGameEntity* baseEntity = EntityManager::Instance().GetEntity(playerID);
	    dynamic_cast<Player*>(baseEntity)->SetPosition(newPosition);

	}
}
Walk* Walk::m_instance = nullptr; // Initialize the static instance pointerq