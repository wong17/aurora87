#include "Shoot.h"
#include "AIEnviroment/Equipaments/Guns/AK47_Entity.h"
#include "AIEnviroment/Equipaments/Guns/GunsMechanics/AK47_Mechanics/Idle.h"
namespace AIEnviroment {
	Shoot* AIEnviroment::Shoot::Instance()
	{
		if (!m_instance) {
			m_instance = new Shoot();
		}
		return m_instance;
	}

	void AIEnviroment::Shoot::Enter(AK47_Entity* entity)
	{//std::cout << "AK47 Shoot state entered." << std::endl;
		entity->SetSpeed(0.0f); // Stop the entity while shooting
		entity->SetIsShooting(true);
		entity->SetIsLoading(false);
		entity->SetAttackCooldownTimer(entity->GetAttackCooldownTimer() + entity->GetDeltaTime());

		if (entity->GetAttackCooldownTimer() >= entity->GetAttackCooldown())
		{
			EventData shootEvent(EventType::AK47_SHOOT);
			shootEvent.Set("AK47ID", entity->GetID());
			EventManager::GetInstance()->DispatchEvent(shootEvent);
			entity->SetAttackCooldownTimer(0.0f); // Reset cooldown timer
		}
	}

	void AIEnviroment::Shoot::Execute(AK47_Entity* entity)
	{
		//std::cout << "AK47 Shoot state executing." << std::endl;
		if (entity->IsShooting())
		{
			entity->SetSpeed(0.0f); // Ensure the entity remains stationary while shooting
			entity->SetIsLoading(false);
			entity->SetIsShooting(true);
			entity->SetAttackCooldownTimer(entity->GetAttackCooldownTimer() + entity->GetDeltaTime());
			if (entity->GetAttackCooldownTimer() >= entity->GetAttackCooldown())
			{
				EventData shootEvent(EventType::AK47_SHOOT);
				shootEvent.Set("AK47ID", entity->GetID());
				EventManager::GetInstance()->DispatchEvent(shootEvent);
				entity->SetAttackCooldownTimer(0.0f); // Reset cooldown timer 
			}
		}
		else
		{
			entity->ChangeFiniteStateAK7(AIEnviroment::Idle::Instance());
		}
	}

	void AIEnviroment::Shoot::Exit(AK47_Entity* entity)
	{
	}
	Shoot* Shoot::m_instance = nullptr;
}