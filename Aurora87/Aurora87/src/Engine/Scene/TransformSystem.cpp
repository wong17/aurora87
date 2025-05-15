#include "TransformSystem.h"

namespace Engine
{
	void TransformSystem::TranslateEntity(const std::shared_ptr<Entity>& entity, const glm::vec3& vector)
	{
		_ApplyToEntity(entity, [&](Entity& ent) { ent.Translate(vector); }, "TranslateEntity(ptr)");
	}

	void TransformSystem::TranslateEntity(Engine::Entity::EntityID id, const glm::vec3& vector)
	{
		_ApplyToEntity(id, [&](Entity& ent) { ent.Translate(vector); }, "TranslateEntity(ID)");
	}

	void TransformSystem::TranslateEntity(const std::string& name, const glm::vec3& vector)
	{
		_ApplyToEntity(name, [&](Entity& ent) { ent.Translate(vector); }, "TranslateEntity(name)");
	}

	void TransformSystem::TranslateEntities(const std::vector<std::shared_ptr<Entity>>& entities, const glm::vec3& delta)
	{
		for (auto& e : entities)
			TranslateEntity(e, delta);
	}

	void TransformSystem::RotateEntity(const std::shared_ptr<Entity>& entity, const glm::vec3& angles)
	{
		_ApplyToEntity(entity, [&](Entity& ent) { ent.Rotate(angles); }, "RotateEntity(ptr)");
	}

	void TransformSystem::RotateEntity(Engine::Entity::EntityID id, const glm::vec3& angles)
	{
		_ApplyToEntity(id, [&](Entity& ent) { ent.Rotate(angles); }, "RotateEntity(ID)");
	}

	void TransformSystem::RotateEntity(const std::string& name, const glm::vec3& angles)
	{
		_ApplyToEntity(name, [&](Entity& ent) { ent.Rotate(angles); }, "RotateEntity(name)");
	}

	void TransformSystem::RotateEntities(const std::vector<std::shared_ptr<Entity>>& entities, const glm::vec3& delta)
	{
		for (auto& e : entities)
			RotateEntity(e, delta);
	}

	void TransformSystem::ScaleEntity(const std::shared_ptr<Entity>& entity, const glm::vec3& factor)
	{
		_ApplyToEntity(entity, [&](Entity& ent) { ent.Scale(factor); }, "ScaleEntity(ptr)");
	}

	void TransformSystem::ScaleEntity(Engine::Entity::EntityID id, const glm::vec3& factor)
	{
		_ApplyToEntity(id, [&](Entity& ent) { ent.Scale(factor); }, "ScaleEntity(ID)");
	}

	void TransformSystem::ScaleEntity(const std::string& name, const glm::vec3& factor)
	{
		_ApplyToEntity(name, [&](Entity& ent) { ent.Scale(factor); }, "ScaleEntity(name)");
	}

	void TransformSystem::ScaleEntities(const std::vector<std::shared_ptr<Entity>>& entities, const glm::vec3& delta)
	{
		for (auto& e : entities)
			ScaleEntity(e, delta);
	}

	void TransformSystem::SetEntityTransform(const std::shared_ptr<Entity>& entity, const TransformComponent& transform)
	{
		_ApplyToEntity(entity, [&](Entity& ent) { ent.SetTransform(transform); }, "SetTransform(ptr)");
	}

	void TransformSystem::SetEntityTransform(Engine::Entity::EntityID id, const TransformComponent& transform)
	{
		_ApplyToEntity(id, [&](Entity& ent) { ent.SetTransform(transform); }, "SetTransform(ID)");
	}

	void TransformSystem::SetEntityTransform(const std::string& name, const TransformComponent& transform)
	{
		_ApplyToEntity(name, [&](Entity& ent) { ent.SetTransform(transform); }, "SetTransform(name)");
	}

	void TransformSystem::SetEntitiesTransform(const std::vector<std::shared_ptr<Entity>>& entities, const TransformComponent& transform)
	{
		for (auto& e : entities)
			SetEntityTransform(e, transform);
	}
}