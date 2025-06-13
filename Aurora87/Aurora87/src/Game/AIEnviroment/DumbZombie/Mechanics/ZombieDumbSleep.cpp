#include "ZombieDumbSleep.h"

namespace AIEnviroment
{
	ZombieDumbSleep* ZombieDumbSleep::Instance()
	{
		if (!m_instance)
		{
			m_instance = new ZombieDumbSleep();
		}
		return m_instance;
	}
	void ZombieDumbSleep::Enter(DumbZombie* entity)
	{
		entity->ClosestPlayerTarget();
	}
	void ZombieDumbSleep::Execute(DumbZombie* entity)
	{
		entity->ClosestPlayerTarget(); // Find the closest player target

		if (entity->GetPlayerTargetID() != -1)
		{
			//std::cout << "Se encontro un id de jugador cercano, cambio de mecanica de dormir a seguir" << std::endl;
			entity->ChangeFiniteStateDumbZombie(Seek::Instance());
		}
		else
		{
			std::cout << "No hay jugadores cercas, dormir" << std::endl;

			entity->SetSpeed(0.0f);
			entity->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}
	void ZombieDumbSleep::Exit(DumbZombie* entity)
	{

	}
	ZombieDumbSleep* ZombieDumbSleep::m_instance = nullptr; // Initialize the static instance pointer

}