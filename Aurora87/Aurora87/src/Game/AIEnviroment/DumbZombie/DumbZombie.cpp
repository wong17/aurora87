#include "DumbZombie.h"
#include <AIEnviroment/Player/Player.h>
#include <AIEnviroment/DumbZombie/Mechanics/ZombieDumbSleep.h>
namespace AIEnviroment {
	DumbZombie::DumbZombie()
		: BaseGameEntity()
		, m_CurrentStateDumbZombie()
		, m_speed(0.0f)
		, m_health(100.0f)
		, m_position(0.0f, 0.0f, 0.0f)
		, m_direction(0.0f, 0.0f, 0.0f)
		, m_targetPosition(0.0f, 0.0f, 0.0f)
		, m_targetDirection(0.0f, 0.0f, 0.0f)
		, m_targetVelocity(0.0f, 0.0f, 0.0f)
		, m_velocity(0.0f, 0.0f, 0.0f)
		, m_targetPositionOffset(0.0f, 0.0f, 0.0f)
		, AttackRange(0.1f)
		, AttackDamage(15.0f)
		, AttackCooldown(2.0f)
		, AttackCooldownTimer(2.0f)
		, m_playerTargetID(-1)
		, m_deltaTime(0.01666f)
	{
		EventManager::GetInstance()->AddListener(EventType::PLAYER_RESPAWN, this);
		EventManager::GetInstance()->AddListener(EventType::PLAYER_ATTACK, this);
		EventManager::GetInstance()->AddListener(EventType::PLAYER_PUNCH, this);
		EventManager::GetInstance()->AddListener(EventType::ZOMBIE_DEATH, this);
		m_CurrentStateDumbZombie = ZombieDumbSleep::Instance();
		m_CurrentStateDumbZombie->Enter(this);

	}
	DumbZombie::~DumbZombie()
	{
		EventManager::GetInstance()->RemoveListener(EventType::PLAYER_RESPAWN, this);
		EventManager::GetInstance()->RemoveListener(EventType::PLAYER_ATTACK, this);
		EventManager::GetInstance()->RemoveListener(EventType::PLAYER_PUNCH, this);
		EventManager::GetInstance()->RemoveListener(EventType::ZOMBIE_DEATH, this);
		if (m_CurrentStateDumbZombie)
		{
			m_CurrentStateDumbZombie->Exit(this);
		}

	}

	void DumbZombie::Update()
	{

		if (m_CurrentStateDumbZombie)
		{

			m_CurrentStateDumbZombie->Execute(this);

		}
		//std::cout << m_targetPosition.x << ", " << m_targetPosition.y << ", " << m_targetPosition.z << std::endl;
	}

	void DumbZombie::ChangeFiniteStateDumbZombie(FiniteStateDumbZombie* finiteStateDumbZombie)
	{
		assert(m_CurrentStateDumbZombie && finiteStateDumbZombie);
		m_CurrentStateDumbZombie->Exit(this);

		m_CurrentStateDumbZombie = finiteStateDumbZombie;
		m_CurrentStateDumbZombie->Enter(this);
	}
	void DumbZombie::HandleEvent(const EventData& event)
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
					m_CurrentStateDumbZombie->Exit(this);
					m_CurrentStateDumbZombie = ZombieDumbSleep::Instance();
					m_CurrentStateDumbZombie->Enter(this);
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
	void DumbZombie::ClosestPlayerTarget()
	{
		auto& allEntities = AIEnviroment::GameEntityManager::Instance().GetAllEntities();

		double closestDistance = std::numeric_limits<float>::max();
		AIEnviroment::BaseGameEntity* closestPlayer = nullptr;

		for (auto& pair : allEntities) {
			auto* entity = pair.second;

			if (entity->GetID() == this->GetID()) continue; 
			if (entity->GetCategory() != EntityCategory::PLAYER) continue;

			double distance = glm::length(entity->GetPosition() - this->GetPosition());
			double range = m_detectionRadius + entity->BRadius();

			if (distance < range && distance < closestDistance) {
				closestDistance = distance;
				closestPlayer = entity;
			}
		}

		if (closestPlayer) {
			m_targetPosition = closestPlayer->GetPosition();
			m_playerTargetID = closestPlayer->GetID();

		
		}
		else {
			std::cout << "Error: No hay jugador cercano en rango" << std::endl;
		}
	}



	void DumbZombie::Death()
	{
		EventData ZombieDeath(EventType::ZOMBIE_DEATH);
		GameEntityManager::Instance().RemoveEntity(GetID());
		std::cout << GetID() << std::endl;
		EventManager::GetInstance()->DispatchEvent(ZombieDeath);
	}

}