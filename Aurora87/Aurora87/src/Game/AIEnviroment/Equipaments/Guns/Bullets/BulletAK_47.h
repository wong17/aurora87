#pragma once
#include "AIEnviroment/Base/BaseGameEntity.h"
#include "AIEnviroment/Equipaments/Guns/Bullets/BulletManager.h"
namespace AIEnviroment {
	class BulletAK_47 : public BaseGameEntity, public BulletManager
	{
	private:
		bool m_tagged;
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
		float m_deltaTime = 0.0f;
	public:
		BulletAK_47();
		BulletAK_47(float damage, glm::vec3 direction, float range, float speed);
		~BulletAK_47(); 
		void Update();
		void ChangeFiniteStateAK7();
		std::string GetType() const override { return "AK-47"; }
		EntityCategory GetCategory() const override { return EntityCategory::ITEM; }
		double BRadius() const override { return 1.4f; }
		void Tag() override { m_tagged = true; }
		void UnTag() override { m_tagged = false; }
		glm::vec3 GetPosition() const override { return m_position; }
		bool IsTagged() const override { return m_tagged; }
		float GetSpeed() const { return m_speed; }
		void SetSpeed(const float speed) { m_speed = speed; }
		void SetPosition(const glm::vec3& position) { m_position = position; }
		void SetDagame(const float damage) { AttackDamage = damage; }
		float GetDagame() const { return AttackDamage; }
		bool GetIsLoaded() const { return isLoadded; }
		void SetIsLoaded(const bool loaded) { isLoadded = loaded; }
		void SetDirection(const glm::vec3& direction) { m_direction = direction; }
		glm::vec3 GetDirection() const { return m_direction; }
		void SetRange(const float range) { AttackRange = range; }
		float GetRange() const { return AttackRange; }
	};

}