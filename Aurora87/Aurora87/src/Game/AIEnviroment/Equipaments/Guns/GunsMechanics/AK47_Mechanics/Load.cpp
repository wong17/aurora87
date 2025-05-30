#include "Load.h"
#include "AIEnviroment/Equipaments/Guns/AK47_Entity.h"
#include "Idle.h"
#include <AIEnviroment/Equipaments/Guns/Bullets/BulletAK_47.h>
namespace AIEnviroment {
	Load* Load::Instance()
	{
		if (!m_instance)
		{
			m_instance = new Load();
		}
		return m_instance;
	}
	void Load::Enter(AK47_Entity* entity)
	{
		entity->SetIsLoading(true);
		entity->SetIsShooting(false);
	}
	void Load::Execute(AK47_Entity* entity)
	{

		entity->SetLoadingColdownTimer(entity->GetLoadingCooldownTimer() + entity->GetDeltaTime());
		if (entity->GetLoadingCooldownTimer() >= entity->GetLoadingColdown()) {
			for (int i = 0; i <= entity->GetMaxBullets(); i++) {
				BulletAK_47* bullet = new BulletAK_47();
				bullet->SetDagame(entity->GetAttackDamage());
				bullet->SetPosition(entity->GetPosition());
				bullet->SetDirection(entity->GetDirection());
				entity->SetBulletsInCharge(bullet);
				entity->SetCountBullets(entity->GetCountBullets() + 1);
			}
			entity->ChangeFiniteStateAK7(AIEnviroment::Idle::Instance());
		}
	}

	void Load::Exit(AK47_Entity* entity)
	{
		entity->SetIsLoading(false);
	}
	Load* Load::m_instance = nullptr;
} 