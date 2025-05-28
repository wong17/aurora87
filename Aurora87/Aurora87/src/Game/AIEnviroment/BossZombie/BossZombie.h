#pragma once
#include <AIEnviroment/Base/BaseGameEntity.h>
#include "AIEnviroment/EventGame/EventData.h"
#include <AIEnviroment/EventGame/EventManager.h>
#include <glad/glad.h>
#include "FiniteStateBossZombie.h"

namespace AIEnviroment {
	class BossZombie : public BaseGameEntity, public EventListener
	{

	private:
		FiniteStateBossZombie* m_CurrentStateBossZombie;
		float m_speed;
		float m_health;
		glm::vec3 m_position;
		glm::vec3 m_direction;
		glm::vec3 m_targetPosition;
		glm::vec3 m_targetDirection;
		glm::vec3 m_targetVelocity;
		glm::vec3 m_velocity;
		glm::vec3 m_targetPositionOffset;
		float RangeChargeAttack;
		float AttackRange;
		float AttackDamage;
		float AttackCooldown;
		float AttackCooldownTimer;
		float DistanceDamage;
		float DistanceCooldown;
		float DistanceCooldownTimer;
		int m_playerTargetID;
		float m_deltaTime;
		GLuint m_textureID;
		float FleeRange;
		float DistanceAttack;
		float chargeAttackRange;
		float ChargeAttackDamage;
		float ChargeAttackCooldown;
		float ChargeAttackCooldownTimer;
		float ChargeTimer;
		float ChargeTimerMax;
		bool isCharging;
		bool firstTimeCharge;
		float DistanceToPlayerCharge;
		float m_detectionRadius = 30.0f;
	public:
		BossZombie();
		~BossZombie();
		void Update();
		std::string GetType() const override { return "BossZombie"; }
		void ChangeFiniteStateBossZombie(FiniteStateBossZombie* finiteStateBossZombie);
		glm::vec3 GetPosition() const { return m_position; }
		glm::vec3 GetDirection() const { return m_direction; }
		glm::vec3 GetVelocity() const { return m_velocity; }
		glm::vec3 GetTargetPosition() const { return m_targetPosition; }
		glm::vec3 GetTargetDirection() const { return m_targetDirection; }
		glm::vec3 GetTargetVelocity() const { return m_targetVelocity; }
		float GetSpeed() const { return m_speed; }
		float GetHealth() const { return m_health; }
		float GetAttackRange() const { return AttackRange; }
		float GetAttackDamage() const { return AttackDamage; }
		float GetAttackCooldown() const { return AttackCooldown; }
		float GetAttackCooldownTimer() const { return AttackCooldownTimer; }
		void SetPosition(const glm::vec3& position) { m_position = position; }
		virtual void HandleEvent(const EventData& event);
		void SetPlayerTargetID(const int playerID) { m_playerTargetID = playerID; }
		int GetPlayerTargetID() const { return m_playerTargetID; }
		void SetTargetPosition(const glm::vec3& targetPosition) { m_targetPosition = targetPosition; }
		void SetTargetDirection(const glm::vec3& targetDirection) { m_targetDirection = targetDirection; }
		void SetTargetVelocity(const glm::vec3& targetVelocity) { m_targetVelocity = targetVelocity; }
		void SetSpeed(float speed) { m_speed = speed; }
		void SetHealth(float health) { m_health = health; }
		void SetAttackRange(float attackRange) { AttackRange = attackRange; }
		void SetAttackDamage(float attackDamage) { AttackDamage = attackDamage; }
		void SetAttackCooldown(float attackCooldown) { AttackCooldown = attackCooldown; }
		void SetAttackCooldownTimer(float attackCooldownTimer) { AttackCooldownTimer = attackCooldownTimer; }
		void SetTargetPositionOffset(const glm::vec3& targetPositionOffset) { m_targetPositionOffset = targetPositionOffset; }
		glm::vec3 GetTargetPositionOffset() const { return m_targetPositionOffset; }
		void SetDirection(const glm::vec3& direction) { m_direction = direction; }
		void SetVelocity(const glm::vec3& velocity) { m_velocity = velocity; }
		void ClosestPlayerTarget();
		void SetDeltaTime(float deltaTime) { m_deltaTime = deltaTime; }
		float GetDeltaTime() const { return m_deltaTime; }
		void Death();
		void SetTextureID(GLuint texID) { m_textureID = texID; }
		GLuint GetTextureID() const { return m_textureID; }
		float GetFleeRange() const { return FleeRange; }
		void SetFleeRange(float fleeRange) { FleeRange = fleeRange; }
		float GetDistanceAttack() const { return DistanceAttack; }
		void SetDistanceAttack(float distanceAttack) { DistanceAttack = distanceAttack; }
		float GetDistanceDamage() const { return DistanceDamage; }
		void SetDistanceDamage(float distanceDamage) { DistanceDamage = distanceDamage; }
		float GetDistanceCooldown() const { return DistanceCooldown; }
		void SetDistanceCooldown(float distanceCooldown) { DistanceCooldown = distanceCooldown; }
		float GetDistanceCooldownTimer() const { return DistanceCooldownTimer; }
		void SetDistanceCooldownTimer(float distanceCooldownTimer) { DistanceCooldownTimer = distanceCooldownTimer; }
		float GetChargeAttackRange() const { return chargeAttackRange; }
		void SetChargeAttackRange(float chargeAttackRange) { this->chargeAttackRange = chargeAttackRange; }
		float GetChargeAttackDamage() const { return ChargeAttackDamage; }
		void SetChargeAttackDamage(float chargeAttackDamage) { ChargeAttackDamage = chargeAttackDamage; }
		float GetChargeAttackCooldown() const { return ChargeAttackCooldown; }
		void SetChargeAttackCooldown(float chargeAttackCooldown) { ChargeAttackCooldown = chargeAttackCooldown; }
		float GetChargeAttackCooldownTimer() const { return ChargeAttackCooldownTimer; }
		float GetChargeTimer() const { return ChargeTimer; }
		void SetChargeTimer(float chargeTimer) { ChargeTimer = chargeTimer; }
		float GetChargeTimerMax() const { return ChargeTimerMax; }
		void SetChargeTimerMax(float chargeTimerMax) { ChargeTimerMax = chargeTimerMax; }
		void SetChargeAttackCooldownTimer(float chargeAttackCooldownTimer) { ChargeAttackCooldownTimer = chargeAttackCooldownTimer; }
		void SetRangeChargeAttack(float rangeChargeAttack) { RangeChargeAttack = rangeChargeAttack; }
		float GetRangeChargeAttack() const { return RangeChargeAttack; }
		void SetIsCharge(bool isCharge) { isCharging = isCharge; }
		bool GetIsCharge() const { return isCharging; }
		void SetFirstTimeCharge(bool firstTime) { firstTimeCharge = firstTime; }
		bool GetFirstTimeCharge() const { return firstTimeCharge; }
		void SetDistanceToPlayerCharge(float distanceToPlayer) { DistanceToPlayerCharge = distanceToPlayer; }
		float GetDistanceToPlayerCharge() const { return DistanceToPlayerCharge; }
		// Heading is the direction the entity is facing
		glm::vec3 Heading() const {
			if (glm::length(m_velocity) > 0.0001f)
				return glm::normalize(m_velocity);
			else
				return glm::vec3(0.0f, 0.0f, 1.0f);
		}
	};

}