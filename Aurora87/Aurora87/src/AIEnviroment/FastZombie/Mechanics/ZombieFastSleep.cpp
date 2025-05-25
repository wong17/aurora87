#include "ZombieFastSleep.h"
#include "AIEnviroment/FastZombie/Mechanics/SeekFast.h"
#include "AIEnviroment/FastZombie/FastZombie.h"
#include <iostream>

namespace AIEnviroment
{

	ZombieFastSleep* ZombieFastSleep::Instance()
	{
		if (!m_instance) {
			m_instance = new ZombieFastSleep();
		}
		return m_instance;
	}

	void ZombieFastSleep::Enter(FastZombie* entity)
	{
		entity->ClosestPlayerTarget();

	}

	void ZombieFastSleep::Execute(FastZombie* entity)
	{
		entity->ClosestPlayerTarget(); // Find the closest player target

		if (entity->GetPlayerTargetID() != -1)
		{
			//std::cout << "Se encontro un id de jugador cercano, cambio de mecanica de dormir a seguir" << std::endl;
			entity->ChangeFiniteStateFastZombie(SeekFast::Instance());
		}
		else
		{
			std::cout << "No hay jugadores cercas, dormir" << std::endl;

			entity->SetSpeed(0.0f);
			entity->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}

	void ZombieFastSleep::Exit(FastZombie* entity)
	{
	}
	ZombieFastSleep* ZombieFastSleep::m_instance = nullptr;
}