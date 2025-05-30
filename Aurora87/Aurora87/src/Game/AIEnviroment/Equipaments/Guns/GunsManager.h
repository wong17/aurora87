#pragma once
#include "Bullets/BulletManager.h"
namespace AIEnviroment {
	enum class TypesGun {
		NONE,
		AK_47,
	};
	class GunsManager {
	private:
		float m_speed = 0.0f;
		glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 m_direction = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 m_targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 m_targetDirection = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 m_targetVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 m_targetPositionOffset = glm::vec3(0.0f, 0.0f, 0.0f);
		bool isLoadded = false;
		float AttackRange = 0.0f;
		float AttackDamage = 0.0f;
		float AttackCooldown = 0.0f;
		float AttackCooldownTimer = 0.0f;
		float m_deltaTime = 0.0f;
		bool isLoading = false;
		bool isShooting = false;
		bool hasBullets = false;
		int m_countBullets = 0;
		int m_MaxBullets = 0;
		float LoadingColdownTimer = 0.0f;
		float LoadingColdown = 0.0f;
		TypesGun m_Type = TypesGun::NONE;
	public:
		GunsManager() {};
		~GunsManager() = default;
		void SetPosition(const glm::vec3& position) { m_position = position; }
		glm::vec3 GetPosition() const { return m_position; }
		void SetSpeed(float speed) { m_speed = speed; }
		float GetSpeed() const { return m_speed; }
		void SetTargetPosition(const glm::vec3& targetPosition) { m_targetPosition = targetPosition; }
		glm::vec3 GetTargetPosition() const { return m_targetPosition; }
		void SetAttackRange(float range) { AttackRange = range; }
		float GetAttackRange() const { return AttackRange; }
		void SetAttackDamage(float damage) { AttackDamage = damage; }
		float GetAttackDamage() const { return AttackDamage; }
		void SetAttackCooldown(float cooldown) { AttackCooldown = cooldown; }
		float GetAttackCooldown() const { return AttackCooldown; }
		void SetAttackCooldownTimer(float timer) { AttackCooldownTimer = timer; }
		float GetAttackCooldownTimer() const { return AttackCooldownTimer; }
		void SetDeltaTime(float deltaTime) { m_deltaTime = deltaTime; }
		float GetDeltaTime() const { return m_deltaTime; }
		bool IsLoading() const { return isLoading; };
		bool IsShooting() const { return isShooting; }
		void SetIsLoading(bool loading) { isLoading = loading; };
		void SetIsShooting(bool shooting) { isShooting = shooting; }
		void SetType(const TypesGun type) { m_Type = type; }
		TypesGun GetTypeGun()const {return m_Type; }
		void SetHasBullets(bool hasBullet) { hasBullets = hasBullet; };
		bool HasBullets() const { return hasBullets; }
		void SetCountBullets(int count) { m_countBullets = count; };
		int GetCountBullets() const { return m_countBullets; }
		void SetMaxBullets(int maxBullets) { m_MaxBullets = maxBullets; };
		int GetMaxBullets() const { return m_MaxBullets; }
		void SetDirection(const glm::vec3& direction) { m_direction = direction; }
		glm::vec3 GetDirection() const { return m_direction; }
		float GetLoadingCooldownTimer() const { return LoadingColdownTimer; }
		void SetLoadingColdownTimer(const float timer) { LoadingColdownTimer = timer; }
		float GetLoadingColdown() const { return LoadingColdown; }
		void SetLoadingColdown(const float coldown) { LoadingColdown = coldown; }

	};
}