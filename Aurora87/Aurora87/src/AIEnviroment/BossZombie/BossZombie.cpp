#include "BossZombie.h"
#include <iostream>
#include "Mechanics/ZombieSleepBoss.h"
#include <AIEnviroment/Player/Player.h>
namespace AIEnviroment {
	BossZombie::BossZombie() :
		m_CurrentStateBossZombie()
		, m_speed(0.0f)
		, m_health(1000.0f)
		, m_position(0.0f, 0.0f, 0.0f)
		, m_direction(0.0f, 0.0f, 0.0f)
		, m_targetPosition(0.0f, 0.0f, 0.0f)
		, m_targetDirection(0.0f, 0.0f, 0.0f)
		, m_targetVelocity(0.0f, 0.0f, 0.0f)
		, m_velocity(0.0f, 0.0f, 0.0f)
		, m_targetPositionOffset(0.0f, 0.0f, 0.0f)
		, AttackRange(5.0f)
		, AttackDamage(5.0f)
		, AttackCooldown(2.0f)
		, AttackCooldownTimer(2.0f)
		, m_playerTargetID(-1)
		, m_deltaTime(0.01666f)
		, m_textureID(-1)
		, FleeRange(20.0f)
		, DistanceAttack(40.0f)
		, chargeAttackRange(10.0f)
		, ChargeAttackDamage(50.0f)
		, ChargeAttackCooldown(9.0f)
		, ChargeAttackCooldownTimer(0.0f)
		, ChargeTimer(0.0f)
		, ChargeTimerMax(4.0f)
		, DistanceDamage(30.0f)
		, DistanceCooldown(2.0f)
		, DistanceCooldownTimer(0.0f)
		, RangeChargeAttack(10.0f)
		, isCharging(true)
		, firstTimeCharge(true)
		, DistanceToPlayerCharge(30.0f)
	{
		EventManager::GetInstance()->AddListener(EventType::PLAYER_RESPAWN, this);
		EventManager::GetInstance()->AddListener(EventType::PLAYER_ATTACK, this);
		EventManager::GetInstance()->AddListener(EventType::PLAYER_PUNCH, this);
		EventManager::GetInstance()->AddListener(EventType::ZOMBIE_DEATH, this);
		m_CurrentStateBossZombie = ZombieSleepBoss::Instance();
		m_CurrentStateBossZombie->Enter(this);
	}

	BossZombie::~BossZombie()
	{

		EventManager::GetInstance()->RemoveListener(EventType::PLAYER_RESPAWN, this);
		EventManager::GetInstance()->RemoveListener(EventType::PLAYER_ATTACK, this);
		EventManager::GetInstance()->RemoveListener(EventType::PLAYER_PUNCH, this);
		EventManager::GetInstance()->RemoveListener(EventType::ZOMBIE_DEATH, this);
		if (m_CurrentStateBossZombie)
		{
			m_CurrentStateBossZombie->Exit(this);
		}
	}

	void BossZombie::Update()
	{
		if (m_CurrentStateBossZombie)
		{

			m_CurrentStateBossZombie->Execute(this);

		}
	}

	void BossZombie::ChangeFiniteStateBossZombie(FiniteStateBossZombie* finiteStateBossZombie)
	{
		assert(m_CurrentStateBossZombie && finiteStateBossZombie);
		m_CurrentStateBossZombie->Exit(this);

		m_CurrentStateBossZombie = finiteStateBossZombie;
		m_CurrentStateBossZombie->Enter(this);
	}

	void BossZombie::HandleEvent(const EventData& event)
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
					m_CurrentStateBossZombie->Exit(this);
					m_CurrentStateBossZombie = ZombieSleepBoss::Instance();
					m_CurrentStateBossZombie->Enter(this);
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

	void BossZombie::ClosestPlayerTarget()
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

	void BossZombie::Death()
	{
		EventData ZombieDeath(EventType::ZOMBIE_DEATH);
		GameEntityManager::Instance().RemoveEntity(GetID());
		std::cout << GetID() << std::endl;
		EventManager::GetInstance()->DispatchEvent(ZombieDeath);
	}
}