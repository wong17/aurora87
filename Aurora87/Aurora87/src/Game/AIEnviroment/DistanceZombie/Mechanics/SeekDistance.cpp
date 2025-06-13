#include "SeekDistance.h"

namespace AIEnviroment 
{
	SeekDistance* SeekDistance::Instance()
	{
		if (!m_instance)
		{
			m_instance = new SeekDistance();
		}
		return m_instance;
	}

	void SeekDistance::Enter(DistanceZombie* entity)
	{
		entity->ClosestPlayerTarget(); // Find the closest player target
		//std::cout << "Seguir inicio, buscando id de jugador cercano" << std::endl;
		//std::cout << "ID del jugador: " << entity->GetPlayerTargetID() << std::endl;
		//std::cout << "ID del zombi: " << entity->GetID() << std::endl;
		//std::cout << "Posicion del zombi: " << entity->GetPosition().x << ", " << entity->GetPosition().y << ", " << entity->GetPosition().z << std::endl;
		//std::cout << "Posicion del jugador: " << entity->GetTargetPosition().x << ", " << entity->GetTargetPosition().y << ", " << entity->GetTargetPosition().z << std::endl;
	}

	void SeekDistance::Execute(DistanceZombie* entity)
	{
		entity->SetSpeed(8.0f);
		entity->ClosestPlayerTarget();
		glm::vec3 targetPosition = entity->GetTargetPosition();
		glm::vec3 currentPosition = entity->GetPosition();

		glm::vec3 direction = glm::normalize(targetPosition - currentPosition);

		glm::vec3 desiredVelocity = direction * entity->GetSpeed();
		//std::cout << "Zombie " << entity->GetID()<< " ID Position: " << entity->GetPosition().x << ", " << entity->GetPosition().y << ", " << entity->GetPosition().z << std::endl;

		float distanceFlee = glm::length(currentPosition - targetPosition);;
		if (distanceFlee < entity->GetFleeRange())
		{
			entity->ChangeFiniteStateDumbZombie(FleeDistance::Instance());
		}
		else {
			float distanceToPlayer = glm::length(targetPosition - currentPosition);
			if (distanceToPlayer > entity->GetAttackRange()) {
				glm::vec3 newPosition = currentPosition + desiredVelocity * entity->GetDeltaTime();
				entity->SetPosition(newPosition);
			}
			if (distanceToPlayer <= entity->GetAttackRange())
			{
				entity->SetAttackCooldownTimer(entity->GetAttackCooldownTimer() + entity->GetDeltaTime());
				EventData attackEvent(EventType::ZOMBIE_ATTACK);
				attackEvent.Set("zombieDumbDamage", entity->GetAttackDamage());
				if (entity->GetAttackCooldownTimer() >= entity->GetAttackCooldown())
				{
					attackEvent.Set("zombieID", entity->GetID());
					attackEvent.Set("playerID", entity->GetPlayerTargetID());
					EventManager::GetInstance()->DispatchEvent(attackEvent);
					entity->SetAttackCooldownTimer(0.0f); // Reset cooldown timer
				}

			}
		}
	}

	void SeekDistance::Exit(DistanceZombie* entity)
	{
	}
	SeekDistance* SeekDistance::m_instance = nullptr; // Initialize the static instance pointer
}