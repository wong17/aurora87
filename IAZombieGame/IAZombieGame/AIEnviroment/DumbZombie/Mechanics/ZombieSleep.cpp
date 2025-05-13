#include "ZombieSleep.h"
#include "DumbZombie.h"
#include "Seek.h"
#include <iostream>
#include <Player/Player.h>
ZombieSleep* ZombieSleep::Instance()
{
	if (!m_instance)
	{
		m_instance = new ZombieSleep();
	}
	return m_instance;
}
void ZombieSleep::Enter(DumbZombie* entity)  
{  
   entity->ClosestPlayerTarget();  
}
void ZombieSleep::Execute(DumbZombie* entity)
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
void ZombieSleep::Exit(DumbZombie* entity)
{

}
ZombieSleep* ZombieSleep::m_instance = nullptr; // Initialize the static instance pointer