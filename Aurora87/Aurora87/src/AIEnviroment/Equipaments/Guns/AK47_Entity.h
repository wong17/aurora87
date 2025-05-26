#pragma once
#include <vector>
#include "Bullets/Bullets.h"
#include <AIEnviroment/Base/BaseGameEntity.h>
#include "AIEnviroment/EventGame/EventData.h"
#include <AIEnviroment/EventGame/EventManager.h>
#include <AIEnviroment/Equipaments/Guns/GunsMechanics/FiniteStateAK47.h>
namespace AIEnviroment {
	class AK47_Entity : public BaseGameEntity
	{
	private:
		std::vector<Bullets> m_Bullets;
		FiniteStateAK7* m_CurrentStateAK7;
		float m_speed;
		float m_health;
		glm::vec3 m_position;
		glm::vec3 m_direction;
		glm::vec3 m_targetPosition;
		glm::vec3 m_targetDirection;
		glm::vec3 m_targetVelocity;
		glm::vec3 m_velocity;
		glm::vec3 m_targetPositionOffset;
		float AttackRange;
		float AttackDamage;
		float AttackCooldown;
		float AttackCooldownTimer;
		float m_deltaTime;
		bool isLoading;
		bool isShooting;
	public:

	};
}