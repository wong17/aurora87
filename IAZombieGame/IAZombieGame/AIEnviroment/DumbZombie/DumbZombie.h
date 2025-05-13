#include<DumbZombie/FiniteStateDumbZombie.h>
#include<Base/BaseGameEntity.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include "EventManager.h"
#include "EventData.h"
#include "glad/glad.h"
#pragma once
class DumbZombie : public BaseGameEntity, public EventListener
{
private:
	FiniteStateDumbZombie* m_CurrentStateDumbZombie;
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
	int m_playerTargetID;
	float m_deltaTime;
	GLuint m_textureID;
public:
	DumbZombie();
	~DumbZombie();
	void Update();
	std::string GetType() const override { return "DumbZombie"; }
	void ChangeFiniteStateDumbZombie(FiniteStateDumbZombie* finiteStateDumbZombie);
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

};

// This class represents a dumb zombie entity in the game. It inherits from BaseGameEntity and EventListener.