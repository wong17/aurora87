#include "DistanceZombie.h"
#include <iostream>
#include "Mechanics/ZombieSleepDistance.h"
#include <AIEnviroment/Player/Player.h>

namespace AIEnviroment {
	DistanceZombie::DistanceZombie() :
		m_CurrentStateDistanceZombie()
		, m_speed(0.0f)
		, m_health(100.0f)
		, m_position(0.0f, 0.0f, 0.0f)
		, m_direction(0.0f, 0.0f, 0.0f)
		, m_targetPosition(0.0f, 0.0f, 0.0f)
		, m_targetDirection(0.0f, 0.0f, 0.0f)
		, m_targetVelocity(0.0f, 0.0f, 0.0f)
		, m_velocity(0.0f, 0.0f, 0.0f)
		, m_targetPositionOffset(0.0f, 0.0f, 0.0f)
		, AttackRange(30.0f)
		, AttackDamage(25.0f)
		, AttackCooldown(2.0f)
		, AttackCooldownTimer(2.0f)
		, m_playerTargetID(-1)
		, m_deltaTime(0.01666f)
		, m_textureID(-1)
		, FleeRange(20.0f)

	{
		EventManager::GetInstance()->AddListener(EventType::PLAYER_RESPAWN, this);
		EventManager::GetInstance()->AddListener(EventType::PLAYER_ATTACK, this);
		EventManager::GetInstance()->AddListener(EventType::PLAYER_PUNCH, this);
		EventManager::GetInstance()->AddListener(EventType::ZOMBIE_DEATH, this);
		m_CurrentStateDistanceZombie = ZombieSleepDistance::Instance();
		m_CurrentStateDistanceZombie->Enter(this);
	}

	DistanceZombie::~DistanceZombie()
	{
		EventManager::GetInstance()->RemoveListener(EventType::PLAYER_RESPAWN, this);
		EventManager::GetInstance()->RemoveListener(EventType::PLAYER_ATTACK, this);
		EventManager::GetInstance()->RemoveListener(EventType::PLAYER_PUNCH, this);
		EventManager::GetInstance()->RemoveListener(EventType::ZOMBIE_DEATH, this);
		if (m_CurrentStateDistanceZombie)
		{
			m_CurrentStateDistanceZombie->Exit(this);
		}
	}

	void DistanceZombie::Update()
	{

		if (m_CurrentStateDistanceZombie)
		{

			m_CurrentStateDistanceZombie->Execute(this);

		}
	}

	void DistanceZombie::ChangeFiniteStateDumbZombie(FiniteStateDistanceZombie* finiteStateDistanceZombie)
	{
		assert(m_CurrentStateDistanceZombie && finiteStateDistanceZombie);
		m_CurrentStateDistanceZombie->Exit(this);

		m_CurrentStateDistanceZombie = finiteStateDistanceZombie;
		m_CurrentStateDistanceZombie->Enter(this);
	}

	void DistanceZombie::HandleEvent(const EventData& event)
	{
		if (event.type == EventType::PLAYER_RESPAWN)
		{
			if (event.HasKeyOfType<int>("playerRespawnID"))
			{
				int playerID = event.GetInt("playerRespawnID");
				if (m_playerTargetID == playerID)
				{
					//std::cout << playerID << " ID del jugador que murio" << std::endl;
					//std::cout << "El jugador ha sido respawned, el zombi se detiene." << std::endl;
					m_CurrentStateDistanceZombie->Exit(this);
					m_CurrentStateDistanceZombie = ZombieSleepDistance::Instance();
					m_CurrentStateDistanceZombie->Enter(this);
				}

			}

		}
		if (event.type == EventType::PLAYER_PUNCH) {
			if (event.HasKeyOfType<int>("ZombiePunchID")) {
				int ZombieID = event.GetInt("ZombiePunchID");
				if (GetID() == ZombieID) {

					m_health -= event.GetFloat("PlayerPunchDamage");
					std::cout << "Vida" << std::endl;

					std::cout << m_health << std::endl;

					if (m_health <= 0) {
						std::cout << "Zombie muerto" << std::endl;
						Death();

					}

				}

			}

		}

	}

	void DistanceZombie::ClosestPlayerTarget()
	{
		float closestDistance = std::numeric_limits<float>::max();
		Player* ClosestPlayer = nullptr;
		glm::vec3 currentPosition = this->GetPosition();

		for (const auto& pair : GameEntityManager::Instance().GetAllEntities())
		{
			Player* entity = dynamic_cast<Player*>(pair.second);
			if (entity && entity->GetType() == "Player")
			{
				float distance = glm::length(entity->GetPosition() - currentPosition);
				if (distance < closestDistance)
				{
					closestDistance = distance;

					ClosestPlayer = entity;
				}
			}

		}
		if (ClosestPlayer)
		{
			m_playerTargetID = ClosestPlayer->GetID();
			m_targetPosition = ClosestPlayer->GetPosition();
		}
		else {
			std::cout << "Error:No hay id de jugador" << std::endl;
		}
	}

	void DistanceZombie::Death()
	{
		EventData ZombieDeath(EventType::ZOMBIE_DEATH);
		GameEntityManager::Instance().RemoveEntity(GetID());
		std::cout << GetID() << std::endl;
		EventManager::GetInstance()->DispatchEvent(ZombieDeath);
	}
}