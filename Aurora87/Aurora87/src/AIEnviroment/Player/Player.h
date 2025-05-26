#pragma once
#include <AIEnviroment/Base/BaseGameEntity.h>
#include <AIEnviroment/Base/GameEntityManager.h>
#include <AIEnviroment/EventGame/EventData.h>
#include <AIEnviroment/EventGame/EventManager.h>
#include <AIEnviroment/Player/FiniteStatePlayer.h>
namespace AIEnviroment
{
	class Player : public BaseGameEntity, public EventListener
	{
	private:
		 FiniteStatePlayer* m_CurrentStatePlayer;
		glm::vec3 m_position;
		glm::vec3 m_direction;
		glm::vec3 m_velocity;
		glm::vec3 m_targetPosition;
		glm::vec3 m_targetDirection;
		glm::vec3 m_targetVelocity;
		float m_speed;
		float m_health;
		float m_attackRange;
		float m_attackDamage;
		float m_attackCooldown;
		float m_attackCooldownTimer;
		int m_zombieTargetID;
		bool m_tagged = false;
		float m_deltaTime;
		float m_distanceToZombie;
	public:

		Player();
		~Player();
		void Update();
		void ChangeFiniteStatePlayer(FiniteStatePlayer* finiteStateDumbZombie);

		glm::vec3 GetPosition() const override { return m_position; }
		double BRadius() const override { return 10.0f; }
		void Tag() override { m_tagged = true; }
		void UnTag() override { m_tagged = false; }
		bool IsTagged() const override { return m_tagged; }
		EntityCategory GetCategory() const override { return EntityCategory::PLAYER; }
		std::string GetType() const override { return "Player"; }
		void SetPosition(const glm::vec3& position) { m_position = position; }
		void SetDirection(const glm::vec3& direction) { m_direction = direction; }
		glm::vec3 GetDirection() const { return m_direction; }
		void SetVelocity(const glm::vec3& velocity) { m_velocity = velocity; }
		glm::vec3 GetVelocity() const { return m_velocity; }
		void SetTargetPosition(const glm::vec3& targetPosition) { m_targetPosition = targetPosition; }
		glm::vec3 GetTargetPosition() const { return m_targetPosition; }
		void SetTargetDirection(const glm::vec3& targetDirection) { m_targetDirection = targetDirection; }
		glm::vec3 GetTargetDirection() const { return m_targetDirection; }
		void SetTargetVelocity(const glm::vec3& targetVelocity) { m_targetVelocity = targetVelocity; }
		glm::vec3 GetTargetVelocity() const { return m_targetVelocity; }
		float GetSpeed() const { return m_speed; }
		void SetSpeed(float speed) { m_speed = speed; }
		float GetHealth() const { return m_health; }
		float GetAttackRange() const { return m_attackRange; }
		float GetAttackDamage() const { return m_attackDamage; }
		float GetAttackCooldown() const { return m_attackCooldown; }
		float GetAttackCooldownTimer() const { return m_attackCooldownTimer; }
		void SetAttackCooldonwTimer(const float AttackCooldownTimer) { m_attackCooldownTimer = AttackCooldownTimer; }
		void SetAttackDamageRange(const float AttackDamageRange) { m_attackRange = AttackDamageRange; }
		void SetAttackDamage(const float AttackDamage) { m_attackDamage = AttackDamage; }
		float GetDeltaTime() const { return m_deltaTime; }
		void SetDeltaTime(const float deltaTime) { m_deltaTime = deltaTime; }
		int GetZombieTargetID() const { return m_zombieTargetID; }
		void SetZombieTargetID(const int zombieTargetID) { m_zombieTargetID = zombieTargetID; }
		float GetDistanceToZombie() const { return m_distanceToZombie; }
		void SetDistanceToZombie(const float distanceToZombie) { m_distanceToZombie = distanceToZombie; }
		void Respawn();
		void AttackHandsToHands();
		virtual void HandleEvent(const EventData& event);
		float Dot(glm::vec3 Heading) { return glm::dot(Heading, Heading); }
		glm::vec3 Heading() const {
			if (glm::length(m_velocity) > 0.0001f)
				return glm::normalize(m_velocity);
			else
				return glm::vec3(0.0f, 0.0f, 1.0f);
		}
	};

}