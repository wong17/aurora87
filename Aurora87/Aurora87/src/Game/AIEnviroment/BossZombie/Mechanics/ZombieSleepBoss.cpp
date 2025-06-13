#include "ZombieSleepBoss.h"

namespace AIEnviroment 
{
	ZombieSleepBoss* ZombieSleepBoss::Instance()
	{
		if (!m_instance)
		{
			m_instance = new ZombieSleepBoss();
		}
		return m_instance;
	}

	void ZombieSleepBoss::Enter(BossZombie* entity)
	{
		entity->ClosestPlayerTarget();
	}

	void ZombieSleepBoss::Execute(BossZombie* entity)
	{
		entity->ClosestPlayerTarget(); // Find the closest player target

		if (entity->GetPlayerTargetID() != -1)
		{
			//std::cout << "Se encontro un id de jugador cercano, cambio de mecanica de dormir a seguir" << std::endl;
			entity->ChangeFiniteStateBossZombie(SeekBoss::Instance());
		}
		else
		{
			std::cout << "No hay jugadores cercas, dormir" << std::endl;

			entity->SetSpeed(0.0f);
			entity->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}

	void ZombieSleepBoss::Exit(BossZombie* entity)
	{
	}
	ZombieSleepBoss* ZombieSleepBoss::m_instance = nullptr;
}