#include "Walk.h"

namespace AIEnviroment
{
	Walk::Walk()
	{
		EventManager::GetInstance()->AddListener(EventType::PLAYER_MOVE, this);
		EventManager::GetInstance()->AddListener(EventType::PLAYER_SPEED, this);

	}Walk::~Walk()
	{
		EventManager::GetInstance()->RemoveListener(EventType::PLAYER_MOVE, this);
		EventManager::GetInstance()->RemoveListener(EventType::PLAYER_SPEED, this);


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
	}

	void Walk::Execute(Player* entity)
	{
		glm::vec3 targetPosition = entity->GetTargetPosition();
		glm::vec3 currentPosition = entity->GetPosition();

		entity->SetDistanceToZombie(glm::length(targetPosition - currentPosition));
		if (entity->GetDistanceToZombie() <= entity->GetAttackRange()) {
		}
	}

	void Walk::Exit(Player* entity)
	{
	}
	void Walk::HandleEvent(const EventData& event)
	{
		if (event.type == AIEnviroment::EventType::PLAYER_MOVE)
		{
			glm::vec3 newPosition = {
				event.GetFloat("newPositionX"),
				event.GetFloat("newPositiony"),
				event.GetFloat("newPositionz")
			};
			int playerID = event.GetInt("playerMovedID");
			BaseGameEntity* baseEntity = GameEntityManager::Instance().GetEntity(playerID);
			dynamic_cast<Player*>(baseEntity)->SetPosition(newPosition);
	/*		std::cout << "Player ID: " << playerID << " moved to position: "
				<< newPosition.x << ", " << newPosition.y << ", " << newPosition.z << std::endl;
			*/

		}
		if (event.type == AIEnviroment::EventType::PLAYER_SPEED)
		{
			int playerID = event.GetInt("playerMovedID");
			BaseGameEntity* baseEntity = GameEntityManager::Instance().GetEntity(playerID);
			dynamic_cast<Player*>(baseEntity)->SetSpeed(20.0);

		}
	}
	Walk* Walk::m_instance = nullptr; // Initialize the static instance pointerq
}
