#include "ChargeBoss.h"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "AIEnviroment/BossZombie/BossZombie.h"
#include "SeekBoss.h"
#include <AIEnviroment/Player/Player.h>
namespace AIEnviroment {
	ChargeBoss* ChargeBoss::Instance()
	{
		if (!m_instance)
		{
			m_instance = new ChargeBoss();
		}
		return m_instance;
	}

	void ChargeBoss::Enter(BossZombie* entity)
	{
		std::cout << "ChargeBoss" << std::endl;
		entity->SetSpeed(40.0f);
		entity->ClosestPlayerTarget();
		Player* entityPlayer = dynamic_cast<Player*>(GameEntityManager::Instance().GetEntity(entity->GetPlayerTargetID()));
		glm::vec3 toPlayer = entityPlayer->GetPosition() - entity->GetPosition();
		glm::vec3 directionCharge = glm::normalize(toPlayer);

		entity->SetTargetDirection(directionCharge);
		entity->SetIsCharge(true);
		entity->SetFirstTimeCharge(true);
		entity->SetChargeTimer(0.0f);

	}

	void ChargeBoss::Execute(BossZombie* entity)
	{
		entity->ClosestPlayerTarget(); // Find the closest player target
		if (entity->GetIsCharge() == false)
			return;
		Player* entityPlayer = dynamic_cast<Player*>(GameEntityManager::Instance().GetEntity(entity->GetPlayerTargetID()));
		glm::vec3 currentPosition = entity->GetPosition();

		glm::vec3 direction = entity->GetTargetDirection();
		entity->SetDirection(direction);
		glm::vec3 desiredVelocity = direction * entity->GetSpeed();
		entity->SetTargetVelocity(desiredVelocity);
		//std::cout << "Zombie " << entity->GetID()<< " ID Position: " << entity->GetPosition().x << ", " << entity->GetPosition().y << ", " << entity->GetPosition().z << std::endl;

		float distanceToPlayer = glm::length(entityPlayer->GetPosition() - currentPosition);
		glm::vec3 newPosition = currentPosition + desiredVelocity * entity->GetDeltaTime();
		entity->SetPosition(newPosition);

		if (distanceToPlayer <= entity->GetRangeChargeAttack() && entity->GetFirstTimeCharge())
		{
			EventData attackEvent(EventType::ZOMBIE_CHARGE_ATTACK);
			attackEvent.Set("zombieChargeDamage", entity->GetChargeAttackDamage());
			attackEvent.Set("zombieID", entity->GetID());
			attackEvent.Set("playerID", entity->GetPlayerTargetID());
			EventManager::GetInstance()->DispatchEvent(attackEvent);
			std::cout << "Damage Charge" << " " << entity->GetChargeAttackDamage() << std::endl;

			entity->SetFirstTimeCharge(false);
		}
		entity->SetChargeTimer(entity->GetChargeTimer() + entity->GetDeltaTime());
		if (entity->GetChargeTimer() >= entity->GetChargeTimerMax())
		{
			entity->SetIsCharge(false);
			entity->SetChargeTimer(0.0f);
			entity->ChangeFiniteStateBossZombie(SeekBoss::Instance());
		}

	}

	void ChargeBoss::Exit(BossZombie* entity)
	{
		entity->SetIsCharge(false);
		entity->SetChargeTimer(0.0f);
		entity->SetSpeed(8.0f);
		entity->SetTargetPositionOffset(glm::vec3(0.0f, 0.0f, 0.0f));
		entity->SetTargetDirection(glm::vec3(0.0f, 0.0f, 1.0f));
		entity->SetTargetVelocity(glm::vec3(0.0f, 0.0f, 1.0f));
		std::cout << "Exit ChargeBoss" << std::endl;
		entity->SetFirstTimeCharge(true);
	}
	ChargeBoss* ChargeBoss::m_instance = nullptr;
}