#pragma once
#pragma once  
#include "Entity.h"  
#include <unordered_map>  
#include <typeindex>  
#include <memory>  

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

        // Estructura para almacenar informaci�n de cada entidad.
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
        template<typename T>
        T& GetComponent(const std::shared_ptr<Entity>& entity) {
            auto type = std::type_index(typeid(T));
            if (m_Components.find(type) != m_Components.end() &&
                m_Components[type].find(entity->GetID()) != m_Components[type].end()) {
                return *std::static_pointer_cast<T>(m_Components[type][entity->GetID()]);
            }
            throw std::runtime_error("Component not found!");
        }

        template<typename T>
        void AddComponent(const std::shared_ptr<Entity>& entity, const T& component) {
            auto type = std::type_index(typeid(T));
            m_Components[type][entity->GetID()] = std::make_shared<T>(component);
        }
        explicit EntityManager(UniformBuffer& globalUniformBuffer) : m_GlobalUniformBuffer(globalUniformBuffer) { }

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
        size_t GetEntityCount() const { return m_Records.size(); }

    private:
        // Asigna un �ndice para uso en uniform buffers. Si hay �ndices reciclados disponibles, reutiliza uno;
        // de lo contrario, genera uno nuevo.
        uint32_t AllocateIndex();
        std::unordered_map<std::type_index, std::unordered_map<int, std::shared_ptr<void>>> m_Components;

        // Busca el registro (Record) correspondiente al ID de entidad dado usando b�squeda binaria.
        // Requiere que m_Records est� ordenado por ID de entidad.
        const Record* FindRecordByID(Engine::Entity::EntityID id) const;
        // Versi�n no-const de FindRecordByID, que permite modificar el Record resultante.
        Record* FindRecordByID(Engine::Entity::EntityID id);
		// Genera un ID �nico para una nueva entidad.
        Entity::EntityID GenerateUniqueID() { return m_NextEntityID++; }

        UniformBuffer& m_GlobalUniformBuffer;
        std::vector<Record> m_Records;
        std::unordered_map<std::string, std::shared_ptr<Entity>> m_EntityMap;
        std::unordered_map<Entity::EntityID, std::shared_ptr<Entity>> m_IDMap;
        std::queue<uint32_t> m_FreeIndices;

        uint32_t m_NextIndex = 0;
        Entity::EntityID m_NextEntityID = 1;
    };
}