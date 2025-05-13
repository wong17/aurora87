#include"Base/BaseGameEntity.h"
#include"FiniteStatePlayer.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"EventData.h"
#include"EventManager.h"
#include "Base/EntityManager.h"
#include"shaderClass.h"
#include"Camera.h"
#include "glad/glad.h"
#pragma once
class Player : public BaseGameEntity,public EventListener
{
private:
	GLuint m_textureID;

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
	float m_deltaTime;
	float m_distanceToZombie;
public:
	Player();
	~Player();
	void Update();
	void ChangeFiniteStatePlayer(FiniteStatePlayer* finiteStateDumbZombie);
	std::string GetType() const override { return "Player"; }  
	void SetPosition(const glm::vec3& position)  { m_position = position; }
	glm::vec3 GetPosition() const  { return m_position; }
	void SetDirection(const glm::vec3& direction) { m_direction = direction; }
	glm::vec3 GetDirection() const  { return m_direction; }
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
	int GetZombieTargetID() const {return m_zombieTargetID; }
	void SetZombieTargetID(const int zombieTargetID) { m_zombieTargetID = zombieTargetID; }
	int GetDistanceToZombie() const { return m_distanceToZombie; }
	void SetDistanceToZombie(const float distanceToZombie) { m_distanceToZombie = distanceToZombie; }
	void Respawn();
	void AttackHandsToHands();
	void SetTextureID(GLuint texID) { m_textureID = texID; }
	GLuint GetTextureID() const { return m_textureID; }
	virtual void HandleEvent(const EventData& event);
	void ClosestZombieTarget();


};

