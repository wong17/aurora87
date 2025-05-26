#include "AttackDistanceBoss.h"
#include "AIEnviroment/BossZombie/BossZombie.h"
#include "SeekBoss.h"
#include <AIEnviroment/Player/Player.h>

namespace AIEnviroment {
	AttackDistanceBoss* AttackDistanceBoss::Instance()
	{
		if (!m_instance)
		{
			m_instance = new AttackDistanceBoss();
		}
		return m_instance;
	}

	void AttackDistanceBoss::Enter(BossZombie* entity)
	{
		entity->ClosestPlayerTarget(); // Find the closest player target
	}

	void AttackDistanceBoss::Execute(BossZombie* entity)
	{
		std::cout << "AttackDistanceBoss" << std::endl;
		entity->ClosestPlayerTarget();
		Player* entityPlayer = dynamic_cast<Player*>(GameEntityManager::Instance().GetEntity(entity->GetPlayerTargetID()));
		glm::vec3 targetPosition = entity->GetTargetPosition();
		glm::vec3 currentPosition = entity->GetPosition();
		glm::vec3 direction = glm::normalize(targetPosition - currentPosition);
		entity->SetDirection(direction);
		glm::vec3 desiredVelocity = direction * entity->GetSpeed();
		entity->SetTargetVelocity(desiredVelocity);
		float distanceToPlayer = glm::length(targetPosition - currentPosition);
		if (distanceToPlayer <= entity->GetDistanceAttack())
		{
			entity->SetDistanceCooldownTimer(entity->GetDistanceCooldownTimer() + entity->GetDeltaTime());
			EventData attackEvent(EventType::ZOMBIE_ATTACK);
			attackEvent.Set("zombieDumbDamage", entity->GetDistanceDamage());
			if (entity->GetDistanceCooldownTimer() >= entity->GetDistanceCooldown())
			{
				attackEvent.Set("zombieID", entity->GetID());
				attackEvent.Set("playerID", entity->GetPlayerTargetID());
				EventManager::GetInstance()->DispatchEvent(attackEvent);
				entity->SetDistanceCooldownTimer(0.0f); // Reset cooldown timer
			}
		}
		else {
			entity->ChangeFiniteStateBossZombie(SeekBoss::Instance());
		}
	}

	void AttackDistanceBoss::Exit(BossZombie* entity)
	{
	}
	AttackDistanceBoss* AttackDistanceBoss::m_instance = nullptr;
}