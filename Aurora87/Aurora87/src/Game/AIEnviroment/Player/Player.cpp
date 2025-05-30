#include "Player.h"
#include <AIEnviroment/Player/Mechanics/Walk.h>
#include <random>

namespace AIEnviroment
{
	Player::Player()
		: BaseGameEntity()
		, m_CurrentStatePlayer()
		, m_position(0.0f, 0.0f, 2.0f)
		, m_direction(0.0f, 0.0f, -1.0f)
		, m_velocity(0.0f, 0.0f, 0.0f)
		, m_targetPosition(0.0f, 0.0f, 0.0f)
		, m_targetDirection(0.0f, 0.0f, -1.0f)
		, m_targetVelocity(0.0f, 0.0f, 0.0f)
		, m_speed(0.2)
		, m_health(100.0f)
		, m_attackRange(0.0f)
		, m_attackDamage(0.0f)
		, m_attackCooldown(0.2f)
		, m_attackCooldownTimer(0.2f)
		, m_zombieTargetID(-1)
		, m_deltaTime(0.0f)
		, m_distanceToZombie(0.0)
	{
		EventManager::GetInstance()->AddListener(EventType::PLAYER_CHANGE_GUN, this);
		EventManager::GetInstance()->AddListener(EventType::PLAYER_RESPAWN, this);
		EventManager::GetInstance()->AddListener(EventType::ZOMBIE_ATTACK, this);
		EventManager::GetInstance()->AddListener(EventType::ZOMBIE_CHARGE_ATTACK, this);
		EventManager::GetInstance()->AddListener(EventType::PLAYER_ROTATE, this);
		EventManager::GetInstance()->AddListener(EventType::PLAYER_SPEED, this);

		m_CurrentStatePlayer = Walk::Instance();
	}

	Player::~Player()
	{
		EventManager::GetInstance()->RemoveListener(EventType::PLAYER_RESPAWN, this);
		EventManager::GetInstance()->RemoveListener(EventType::PLAYER_CHANGE_GUN, this);
		EventManager::GetInstance()->RemoveListener(EventType::ZOMBIE_ATTACK, this);
		EventManager::GetInstance()->RemoveListener(EventType::ZOMBIE_CHARGE_ATTACK, this);
		EventManager::GetInstance()->RemoveListener(EventType::PLAYER_ROTATE, this);
		EventManager::GetInstance()->RemoveListener(EventType::PLAYER_SPEED, this);


		if (m_CurrentStatePlayer)
		{
			m_CurrentStatePlayer->Exit(this);
			m_CurrentStatePlayer = nullptr;
		}
	}

	void Player::Update()
	{
		this->SetVelocity(this->GetDirection() * this->GetSpeed());
		if (m_CurrentStatePlayer)
		{
			m_CurrentStatePlayer->Execute(this);
		}
	}

	void Player::ChangeFiniteStatePlayer(FiniteStatePlayer* finiteStatePlayer)
	{
		assert(m_CurrentStatePlayer && finiteStatePlayer);
		m_CurrentStatePlayer->Exit(this);

		m_CurrentStatePlayer = finiteStatePlayer;
		m_CurrentStatePlayer->Enter(this);
	}

	void Player::ChangeGunUsed(GunsManager* currentGun)
	{
		assert(m_CurrentGun && currentGun);
		m_CurrentGun = currentGun;
	}

	void Player::Respawn()
	{
		EventData respawnEvent(EventType::PLAYER_RESPAWN);
		respawnEvent.Set("playerRespawnID", GetID());
		EventManager::GetInstance()->DispatchEvent(respawnEvent);

	}
	void Player::AttackHandsToHands()
	{

	}
	void Player::HandleEvent(const EventData& event)
	{
		if (event.type == EventType::PLAYER_RESPAWN)
		{
			int playerIDRespawn = event.GetInt("playerRespawnID");
			if (GetID() == playerIDRespawn)
			{
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_real_distribution<float> dis(-0.1f, 0.1f);
				float randomX = dis(gen);
				float randomY = dis(gen);
				float randomZ = dis(gen);

				m_health = 100.0f;
				m_position = glm::vec3(randomX, randomY, randomZ);
				EventData respawnEvent(EventType::RANDOM_SPAWN);
				respawnEvent.Set("newPositionX", m_position.x );
				respawnEvent.Set("newPositionY", m_position.y );
				respawnEvent.Set("newPositionZ", m_position.z );
				respawnEvent.Set("playerRandomSpawnID", GetID());
				EventManager::GetInstance()->DispatchEvent(respawnEvent);
			}

		}
		if (event.type == EventType::PLAYER_ROTATE) {
			int playerIDRotate = event.GetInt("playerIDRotate");
			if (GetID() == playerIDRotate) {
				float newOrientationx = event.GetFloat("NewOrientationx");
				float newOrientationy = event.GetFloat("NewOrientationy");
				float newOrientationz = event.GetFloat("NewOrientationz");
				m_direction = glm::vec3(newOrientationx, newOrientationy, newOrientationz);
			}
		}
		if (event.type == EventType::ZOMBIE_ATTACK)
		{
			if (event.HasKeyOfType<int>("playerID"))
			{
				int playerID = event.GetInt("playerID");
				int zombieID = event.GetInt("zombieID");

				if (GetID() == playerID)
				{
					m_health -= event.GetFloat("zombieDumbDamage");
					std::cout << "Vida" << m_health << std::endl;
					std::cout << "Zombie ID: " << zombieID << " Ataco" << std::endl;

					if (m_health <= 0.0f)
					{
						Respawn();
					}
				}
			}
		}
		if (event.type == EventType::ZOMBIE_CHARGE_ATTACK)
		{
			if (event.HasKeyOfType<int>("playerID"))
			{
				int playerID = event.GetInt("playerID");
				int zombieID = event.GetInt("zombieID");
				if (GetID() == playerID)
				{
					m_health -= event.GetFloat("zombieChargeDamage");
					std::cout << "Vida" << m_health << std::endl;
					std::cout << "Zombie ID: " << zombieID << " Ataco con carga" << std::endl;
					if (m_health <= 0.0f)
					{
						Respawn();
					}
				}
			}
		}
		if (event.type == EventType::PLAYER_CHANGE_GUN) {
			int index = event.GetInt("GunIndex");
			switch (index)
			{
			case 0:
				std::cout << "Primera arma cargada" << std::endl;
				break;
			case 1:
				std::cout << "Segunda arma cargada" << std::endl;
				break;
			case 2:
				std::cout << "Tercera arma cargada" << std::endl;
				break;
			case 3:
				std::cout << "Cuarta arma cargada" << std::endl;
				break;
			default:
				break;
			}
		}
	}

	//void Player::ClosestZombieTarget() {
	//	float closestDistance = std::numeric_limits<float>::max();
	//	DumbZombie* ClosestZombie = nullptr;
	//	glm::vec3 currentPosition = this->GetPosition();
	//	for (const auto& pair : EntityManager::Instance().GetAllEntities())
	//	{
	//		DumbZombie* entity = dynamic_cast<DumbZombie*>(pair.second);
	//		if (entity && entity->GetType() == "DumbZombie")
	//		{
	//			float distance = glm::length(entity->GetPosition() - currentPosition);
	//			if (distance < closestDistance)
	//			{
	//				closestDistance = distance;
	//				ClosestZombie = entity;
	//			}

	//		}
	//	}
	//	if (ClosestZombie)
	//	{
	//		m_zombieTargetID = ClosestZombie->GetID();
	//		m_targetPosition = ClosestZombie->GetPosition();

	//	}
	//}
}