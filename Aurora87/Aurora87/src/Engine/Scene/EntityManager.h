#pragma once

#include "Engine/OpenGL/UniformBuffer.h"
#include "Engine/OpenGL/Model.h"
#include "Engine/OpenGL/Mesh.h"
#include "Engine/Scene/Entity.h"
#include "Engine/OpenGL/Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <memory>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstdint>
#include <unordered_map>
#include <iostream>
#include <stdexcept>
#include <initializer_list>

namespace Engine
{
    class EntityManager
    {
    public:
        using EntityID = Entity::EntityID;

        // Estructura para almacenar información de cada entidad.
        struct Record
        {
            std::shared_ptr<Entity> entity;
            uint32_t uniformBufferIndex;
            std::shared_ptr<Shader> shader;
            uint32_t instanceCount = 0;

            bool operator<(const Record& other) const 
            { 
                return entity->GetID() < other.entity->GetID(); 
            }
        };

        explicit EntityManager(UniformBuffer& globalUniformBuffer, UniformBuffer& globalTextureBlockUniformBuffer) 
            : m_GlobalUniformBuffer(globalUniformBuffer), m_GlobalTextureBlockUniformBuffer(globalTextureBlockUniformBuffer)
        { }

        std::shared_ptr<Entity> CreateEntity(
            std::shared_ptr<Model> model, 
            std::shared_ptr<Shader> shader,
            const std::string& name = "");
        std::shared_ptr<Entity> CreateEntity(
            std::shared_ptr<Mesh> mesh, 
            std::shared_ptr<Shader> shader,
            const std::string& name = "");
        void DestroyEntity(const std::shared_ptr<Entity>& entity);
        void DestroyByID(Entity::EntityID id);
        
        void SetInstanceTransforms(
            std::initializer_list<std::pair<std::shared_ptr<Entity>, 
            std::vector<glm::mat4>>> batches,
            const VertexBufferLayout& instanceLayout = Engine::Mesh::InstanceLayout());
        void SetInstanceTransforms(
            const std::string& name, 
            const std::vector<glm::mat4>& mats, 
            const VertexBufferLayout& instanceLayout = Engine::Mesh::InstanceLayout());
        void SetInstanceTransforms(
            const std::shared_ptr<Entity>& entity, 
            const std::vector<glm::mat4>& mats,
            const VertexBufferLayout& instanceLayout = Engine::Mesh::InstanceLayout());
        
        bool Exists(const std::shared_ptr<Entity>& entity) const;
        bool ExistsByID(Entity::EntityID id) const;
        std::shared_ptr<Entity> GetEntityByName(const std::string& name) const;
        std::shared_ptr<Entity> GetEntityByID(Entity::EntityID id) const;

        const std::vector<std::shared_ptr<Entity>> GetAllEntities() const;
        const std::vector<Record>& GetRecords() const { return m_Records; }
        UniformBuffer& GetGlobalUniformBuffer() const { return m_GlobalUniformBuffer; }
        UniformBuffer& GetGlobalTextureBlockUniformBuffer() const { return m_GlobalTextureBlockUniformBuffer; }
        size_t GetEntityCount() const { return m_Records.size(); }

    private:
        // Asigna un índice para uso en uniform buffers. Si hay índices reciclados disponibles, reutiliza uno;
        // de lo contrario, genera uno nuevo.
        uint32_t AllocateIndex();

        // Busca el registro (Record) correspondiente al ID de entidad dado usando búsqueda binaria.
        // Requiere que m_Records esté ordenado por ID de entidad.
        const Record* FindRecordByID(Engine::Entity::EntityID id) const;
        // Versión no-const de FindRecordByID, que permite modificar el Record resultante.
        Record* FindRecordByID(Engine::Entity::EntityID id);
		// Genera un ID único para una nueva entidad.
        Entity::EntityID GenerateUniqueID() { return m_NextEntityID++; }

        UniformBuffer& m_GlobalUniformBuffer;
        UniformBuffer& m_GlobalTextureBlockUniformBuffer;
        std::vector<Record> m_Records;
        std::unordered_map<std::string, std::shared_ptr<Entity>> m_EntityMap;
        std::unordered_map<Entity::EntityID, std::shared_ptr<Entity>> m_IDMap;
        std::queue<uint32_t> m_FreeIndices;

        uint32_t m_NextIndex = 0;
        Entity::EntityID m_NextEntityID = 1;
    };
}