#include "ZombieSleepDistance.h"

namespace AIEnviroment 
{
	ZombieSleepDistance* ZombieSleepDistance::Instance()
	{
		if (!m_instance)
		{
			m_instance = new ZombieSleepDistance();
		}
		return m_instance;
	}

	void ZombieSleepDistance::Enter(DistanceZombie* entity)
	{
		entity->ClosestPlayerTarget();

	}

	void ZombieSleepDistance::Execute(DistanceZombie* entity)
	{
		entity->ClosestPlayerTarget(); // Find the closest player target

		if (entity->GetPlayerTargetID() != -1)
		{
			//std::cout << "Se encontro un id de jugador cercano, cambio de mecanica de dormir a seguir" << std::endl;
			entity->ChangeFiniteStateDumbZombie(SeekDistance::Instance());
		}
		else
		{
			std::cout << "No hay jugadores cercas, dormir" << std::endl;

			entity->SetSpeed(0.0f);
			entity->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}

	void ZombieSleepDistance::Exit(DistanceZombie* entity)
	{

	}
	ZombieSleepDistance* ZombieSleepDistance::m_instance = nullptr; // Initialize the static instance pointer
}