#pragma once

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include <string>

namespace AIEnviroment
{
	enum class EntityCategory 
	{
		PLAYER,
		ZOMBIE,
		OBSTACLE,
		ITEM
	};

	class BaseGameEntity
	{
	public:
		BaseGameEntity() : m_ID(-1) {}
		virtual ~BaseGameEntity() = default;
		virtual std::string GetType() const = 0;
		virtual void Update() = 0;
		int GetID() const { return m_ID; }
		void SetID(int id) { m_ID = id; }
		virtual glm::vec3 GetPosition() const = 0;
		virtual double BRadius() const = 0;
		virtual EntityCategory GetCategory() const = 0;
		// tag methods...
		virtual void Tag() = 0;
		virtual void UnTag() = 0;
		virtual bool IsTagged() const = 0;

	private:
		int m_ID;
	};
}
