#include "EntityManager.h"

namespace Engine
{
    // Crea una nueva entidad y le asigna un índice UBO
    std::shared_ptr<Entity> EntityManager::CreateEntity(std::shared_ptr<Model> model, std::shared_ptr<Shader> shader, const std::string& name)
    {
        uint32_t index = AllocateIndex();
		// Se crea una nueva entidad y se le asigna un ID único
        auto entity = std::make_shared<Entity>(model, shader, name);
        entity->m_ID = GenerateUniqueID();
		// Esto hace que el vector de registros esté ordenado por ID
        Record newRecord{ entity, index, shader };
        auto it = std::lower_bound(m_Records.begin(), m_Records.end(), entity->GetID(), 
            [](const Record& rec, Entity::EntityID id) { return rec.entity->GetID() < id; });
        m_Records.insert(it, std::move(newRecord));

        if (!name.empty())
            m_EntityMap[name] = entity;

        return entity;
    }

    std::shared_ptr<Entity> EntityManager::CreateEntity(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> shader, const std::string& name) 
    {
        uint32_t index = AllocateIndex();
        // Se crea una nueva entidad y se le asigna un ID único
        auto entity = std::make_shared<Entity>(mesh, shader, name);
        entity->m_ID = GenerateUniqueID();
        // Esto hace que el vector de registros esté ordenado por ID
        Record newRecord{ entity, index, shader };
        auto it = std::lower_bound(m_Records.begin(), m_Records.end(), entity->GetID(),
            [](const Record& rec, Entity::EntityID id) { return rec.entity->GetID() < id; });
        m_Records.insert(it, std::move(newRecord));
        
        if (!name.empty())
            m_EntityMap[name] = entity;

        return entity;
    }

    // Destruye una entidad y recicla su índice
    void EntityManager::DestroyEntity(const std::shared_ptr<Entity>& entity)
    {
        auto it = std::find_if(m_Records.begin(), m_Records.end(), [&](auto& rec) { return rec.entity == entity; });
        if (it == m_Records.end())
            return;

        // Reciclar índice del UniformBuffer
        m_FreeIndices.push(it->uniformBufferIndex);

        // Eliminar del mapa solo si coincide el puntero
        const std::string& name = it->entity->GetName();
        auto mapIt = m_EntityMap.find(name);
        if (mapIt != m_EntityMap.end() && mapIt->second == entity)
            m_EntityMap.erase(mapIt);

        // Eliminar de m_Records
        m_Records.erase(it);
    }

    void EntityManager::DestroyByID(Entity::EntityID id)
    {
        Record* rec = FindRecordByID(id);
        if (!rec) return;
        DestroyEntity(rec->entity);
    }

    void EntityManager::SetInstanceTransforms(std::initializer_list<std::pair<std::shared_ptr<Entity>, std::vector<glm::mat4>>> batches, 
        const VertexBufferLayout& instanceLayout)
    {
        for (auto& [entity, mats] : batches) {
            SetInstanceTransforms(entity, mats, instanceLayout);
        }
    }

    void EntityManager::SetInstanceTransforms(const std::string& name, const std::vector<glm::mat4>& mats,
        const VertexBufferLayout& instanceLayout)
    {
		auto entity = GetEntityByName(name);
        if (!entity)
        {
            std::cerr << "EntityManager::SetInstanceTransforms(): No se encontró la entidad con el nombre: " << name << std::endl;
            return;
        }

        SetInstanceTransforms(entity, mats, instanceLayout);
    }

    void EntityManager::SetInstanceTransforms(const std::shared_ptr<Entity>& entity, const std::vector<glm::mat4>& mats, 
        const VertexBufferLayout& instanceLayout)
    {
        if (!entity)
        {
            std::cerr << "EntityManager::SetInstanceTransforms(): La entidad es nula.\n";
            return;
        }

        if (!Exists(entity))
        {
            std::cerr << "EntityManager::SetInstanceTransforms(): La entidad no pertenece al EntityManager.\n";
            return;
        }

        auto it = std::find_if(m_Records.begin(), m_Records.end(), [&](Record const& r) { return r.entity == entity; });
        if (it == m_Records.end()) {
            std::cerr << "EntityManager::SetInstanceTransforms(): Record no encontrado.\n";
            return;
        }
        it->instanceCount = static_cast<uint32_t>(mats.size());

        if (entity->GetMesh() == nullptr && entity->GetModel() == nullptr)
            throw std::runtime_error("EntityManager::SetInstanceTransforms(): La entidad no tiene un Mesh o Model asociado.");

		if (entity->GetMesh() != nullptr)       entity->GetMesh()->SetInstanceTransforms(mats, instanceLayout);
        else if (entity->GetModel() != nullptr) entity->GetModel()->SetInstanceTransforms(mats, instanceLayout);
    }

    bool EntityManager::Exists(const std::shared_ptr<Entity>& entity) const
    {
        return entity && ExistsByID(entity->GetID());
    }

    bool EntityManager::ExistsByID(Engine::Entity::EntityID id) const
    {
        return FindRecordByID(id) != nullptr;
    }

    std::shared_ptr<Entity> EntityManager::GetEntityByName(const std::string& name) const
    {
        auto it = m_EntityMap.find(name);
        return (it != m_EntityMap.end()) ? it->second : nullptr;
    }

    std::shared_ptr<Entity> EntityManager::GetEntityByID(Engine::Entity::EntityID id) const
    {
        const Record* rec = FindRecordByID(id);
        return rec ? rec->entity : nullptr;
    }

    const std::vector<std::shared_ptr<Entity>> EntityManager::GetAllEntities() const
    {
        std::vector<std::shared_ptr<Entity>> result;
        for (const auto& rec : m_Records)
            result.push_back(rec.entity);

        return result;
    }

    uint32_t EntityManager::AllocateIndex()
    {
        if (!m_FreeIndices.empty()) {
            uint32_t idx = m_FreeIndices.front();
            m_FreeIndices.pop();
            return idx;
        }
        return m_NextIndex++;
    }

    const EntityManager::Record * EntityManager::FindRecordByID(Engine::Entity::EntityID id) const
    {
        auto it = std::lower_bound(m_Records.begin(), m_Records.end(), id,
            [id](const Record& rec, Entity::EntityID id) { return rec.entity->GetID() < id; });

        if (it != m_Records.end() && it->entity->GetID() == id)
            return &(*it);

        return nullptr;
    }

    EntityManager::Record* EntityManager::FindRecordByID(Engine::Entity::EntityID id)
    {
        return const_cast<Record*>(static_cast<const EntityManager*>(this)->FindRecordByID(id));
    }
}