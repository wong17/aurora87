#pragma once

#include "Engine/Scene/Entity.h"
#include "Engine/Scene/TransformComponent.h"
#include "Engine/Scene/EntityManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <vector>
#include <initializer_list>
#include <string>
#include <iostream>
#include <algorithm>

namespace Engine
{
	class TransformSystem
	{
    public:
        explicit TransformSystem(EntityManager& entityManager) : m_EntityManager(entityManager) { }

        void TranslateEntity(const std::shared_ptr<Entity>& entity, const glm::vec3& vector);
        void TranslateEntity(Engine::Entity::EntityID id, const glm::vec3& vector);
        void TranslateEntity(const std::string& name, const glm::vec3& vector);
        void TranslateEntities(const std::vector<std::shared_ptr<Entity>>& entities, const glm::vec3& delta);
        void TranslateEntities(std::initializer_list<std::shared_ptr<Entity>> entities, const glm::vec3& delta)
        {
            TranslateEntities(std::vector<std::shared_ptr<Entity>>(entities), delta);
        }

        void RotateEntity(const std::shared_ptr<Entity>& entity, const glm::vec3& angles);
        void RotateEntity(Engine::Entity::EntityID id, const glm::vec3& angles);
        void RotateEntity(const std::string& name, const glm::vec3& angles);
        void RotateEntities(const std::vector<std::shared_ptr<Entity>>& entities, const glm::vec3& delta);
        void RotateEntities(std::initializer_list<std::shared_ptr<Entity>> entities, const glm::vec3& delta)
        {
            RotateEntities(std::vector<std::shared_ptr<Entity>>(entities), delta);
        }

        void ScaleEntity(const std::shared_ptr<Entity>& entity, const glm::vec3& factor);
        void ScaleEntity(Engine::Entity::EntityID id, const glm::vec3& factor);
        void ScaleEntity(const std::string& name, const glm::vec3& factor);
        void ScaleEntities(const std::vector<std::shared_ptr<Entity>>& entities, const glm::vec3& delta);
        void ScaleEntities(std::initializer_list<std::shared_ptr<Entity>> entities, const glm::vec3& delta)
        {
            ScaleEntities(std::vector<std::shared_ptr<Entity>>(entities), delta);
        }

        void SetEntityTransform(const std::shared_ptr<Entity>& entity, const TransformComponent& transform);
        void SetEntityTransform(Engine::Entity::EntityID id, const TransformComponent& transform);
        void SetEntityTransform(const std::string& name, const TransformComponent& transform);
        void SetEntitiesTransform(const std::vector<std::shared_ptr<Entity>>& entities, const TransformComponent& transform);
        void SetEntitiesTransform(std::initializer_list<std::shared_ptr<Entity>> entities, const TransformComponent& transform)
        {
            SetEntitiesTransform(std::vector<std::shared_ptr<Entity>>(entities), transform);
        }
    
    private:
        // Para poder apliar transformaciones a las entidades
        template<typename Key, typename Op>
        void _ApplyToEntity(const Key& key, Op&& op, const char* opName)
        {
            std::shared_ptr<Entity> e = GetByKey(key);
            if (!e) {
                std::cerr << "[TransformSystem:EntityManager] " << opName << ": no existe la entidad para el key dado\n";
                return;
            }
            op(*e);
        }

        EntityManager& m_EntityManager;

        std::shared_ptr<Entity> GetByKey(const std::shared_ptr<Entity>& e) const { return m_EntityManager.Exists(e) ? e : nullptr; }
        std::shared_ptr<Entity> GetByKey(Engine::Entity::EntityID id) const { return m_EntityManager.GetEntityByID(id); }
        std::shared_ptr<Entity> GetByKey(const std::string& name) const { return m_EntityManager.GetEntityByName(name); }
	};
}