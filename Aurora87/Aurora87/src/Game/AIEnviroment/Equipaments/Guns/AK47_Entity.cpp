#include "AK47_Entity.h"
namespace AIEnviroment
{

	AK47_Entity::AK47_Entity()
	{
		m_CurrentStateAK7 = nullptr;

		EventManager::GetInstance()->AddListener(EventType::PLAYER_RESPAWN, this);
		EventManager::GetInstance()->AddListener(EventType::PLAYER_ATTACK, this);
		EventManager::GetInstance()->AddListener(EventType::PLAYER_PUNCH, this);
		EventManager::GetInstance()->AddListener(EventType::ZOMBIE_DEATH, this);
	}

	AK47_Entity::~AK47_Entity()
	{
		EventManager::GetInstance()->RemoveListener(EventType::PLAYER_RESPAWN, this);
		EventManager::GetInstance()->RemoveListener(EventType::PLAYER_ATTACK, this);
		EventManager::GetInstance()->RemoveListener(EventType::PLAYER_PUNCH, this);
		EventManager::GetInstance()->RemoveListener(EventType::ZOMBIE_DEATH, this);
		if (m_CurrentStateAK7)
		{
			m_CurrentStateAK7->Exit(this);
		}
	}

	void AK47_Entity::Update()
	{
		if (m_CurrentStateAK7)
		{
			m_CurrentStateAK7->Execute(this);
		}
	}

	void AK47_Entity::ChangeFiniteStateAK7(FiniteStateAK7* finiteStateAK7)
	{
	}

	void AK47_Entity::HandleEvent(const EventData& eventData)
	{
	}

}