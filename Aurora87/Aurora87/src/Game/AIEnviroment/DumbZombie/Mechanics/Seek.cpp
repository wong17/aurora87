#include "Seek.h"
#include <AIEnviroment/DumbZombie/DumbZombie.h>
#include <AIEnviroment/EventGame/EventManager.h>
#include <AIEnviroment/EventGame/EventData.h>
#include <AIEnviroment/Player/Player.h>
namespace AIEnviroment {
	Seek* Seek::Instance()
	{
		if (!m_instance)
		{
			m_instance = new Seek();
		}
		return m_instance;

	}
	void Seek::Enter(DumbZombie* entity)
	{

		//std::cout << "Seguir inicio, buscando id de jugador cercano" << std::endl;

		entity->ClosestPlayerTarget(); // Find the closest player target

	}

	void Seek::Execute(DumbZombie* entity)
	{
		//std::cout << "ejecutando seguir a jugador" << std::endl;

		entity->SetSpeed(8.0f);
		entity->ClosestPlayerTarget();
		glm::vec3 targetPosition = entity->GetTargetPosition();
		glm::vec3 currentPosition = entity->GetPosition();

		glm::vec3 direction = glm::normalize(targetPosition - currentPosition);

		glm::vec3 desiredVelocity = direction * entity->GetSpeed();
		std::cout << "Zombie " << entity->GetID()<< " ID Position: " << entity->GetPosition().x << ", " << entity->GetPosition().y << ", " << entity->GetPosition().z << std::endl;


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

	void Seek::Exit(DumbZombie* entity)
	{
		entity->SetSpeed(0.0f); // Stop the zombie when exiting the state
		entity->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f)); // Reset velocity
		entity->SetTargetPositionOffset(glm::vec3(0.0f, 0.0f, 0.0f)); // Reset target position offset
	}
	Seek* Seek::m_instance = nullptr; // Initialize the static instance pointer

}