#include "Load.h"
#include "AIEnviroment/Equipaments/Guns/AK47_Entity.h"
#include "Idle.h"
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
			entity->ChangeFiniteStateAK7(AIEnviroment::Idle::Instance());
		}
	}

	void Load::Exit(AK47_Entity* entity)
	{
		entity->SetIsLoading(false);
	}
	Load* Load::m_instance = nullptr;
} 