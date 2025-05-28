#include "FleeDistance.h"
#include "AIEnviroment/DistanceZombie/DistanceZombie.h"
#include <AIEnviroment/Player/Player.h>
#include "SeekDistance.h"

namespace AIEnviroment{
	FleeDistance* FleeDistance::Instance()
	{
		if (!m_instance)
		{
			m_instance = new FleeDistance();
		}
		return m_instance;
	}

	void FleeDistance::Enter(DistanceZombie* entity)
	{
		entity->ClosestPlayerTarget();
		// Find the closest player target
		//std::cout << "FleeDistance" << std::endl;
		//std::cout << "ID del jugador: " << entity->GetPlayerTargetID() << std::endl;
		//std::cout << "ID del zombi: " << entity->GetID() << std::endl;
		//std::cout << "Posicion del zombi: " << entity->GetPosition().x << ", " << entity->GetPosition().y << ", " << entity->GetPosition().z << std::endl;
		//std::cout << "Posicion del jugador: " << entity->GetTargetPosition().x << ", " << entity->GetTargetPosition().y << ", " << entity->GetTargetPosition().z << std::endl;
	}

	void FleeDistance::Execute(DistanceZombie* entity)
	{
		glm::vec3 desideredVelocity = glm::normalize(entity->GetPosition() - entity->GetTargetPosition()) * entity->GetSpeed();
		glm::vec3 FleeZombie = desideredVelocity - entity->GetVelocity();
		glm::vec3 targetPosition = entity->GetTargetPosition();
		glm::vec3 currentPosition = entity->GetPosition();

		glm::vec3 direction = glm::normalize(targetPosition - currentPosition);

		//std::cout << "Zombie " << entity->GetID()<< " ID Position: " << entity->GetPosition().x << ", " << entity->GetPosition().y << ", " << entity->GetPosition().z << std::endl;

		entity->SetVelocity(desideredVelocity);
		float distanceToPlayer = glm::length(currentPosition - targetPosition);
		if (distanceToPlayer < entity->GetFleeRange())
		{
			glm::vec3 newPosition = currentPosition + desideredVelocity * entity->GetDeltaTime();
			entity->SetPosition(newPosition);
		}
		else {
			entity->ChangeFiniteStateDumbZombie(SeekDistance::Instance());
		}
	}

	void FleeDistance::Exit(DistanceZombie* entity)
	{
	}
	FleeDistance* FleeDistance::m_instance = nullptr; // Initialize the static instance pointer
	}