#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <glm/fwd.hpp>
namespace AIEnviroment {

	class BulletManager
	{
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
		float m_deltaTime = 0.0f;
	public:
		BulletManager() {};
		~BulletManager() = default;
		float GetSpeed() const { return m_speed; }
		void SetSpeed(const float speed) { m_speed = speed; }
		void SetPosition(const glm::vec3& position) { m_position = position; }
		glm::vec3 GetPosition() const { return m_position; }
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