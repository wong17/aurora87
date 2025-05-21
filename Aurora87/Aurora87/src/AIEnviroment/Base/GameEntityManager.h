#include<unordered_map>
#include"AIEnviroment/Base/BaseGameEntity.h"
#include<iostream>
#pragma once

namespace AIEnviroment
{

	class GameEntityManager
	{
	private:
		std::unordered_map<int, BaseGameEntity*> m_entities;
		static int m_nextID;
	public:
		GameEntityManager() {
			m_nextID = 1;
		}
		static GameEntityManager& Instance() {
			static GameEntityManager instance;
			return instance;
		}

		void RegisterEntity(BaseGameEntity* entity) {
			entity->SetID(m_nextID);
			std::cout << "Entity ID: " << m_nextID << std::endl;
			m_entities[m_nextID] = entity;
			m_nextID++;
		}

		void RemoveEntity(int entityID) {
			m_entities.erase(entityID);
		}

		BaseGameEntity* GetEntity(int entityID) {
			auto it = m_entities.find(entityID);
			if (it != m_entities.end()) {
				return it->second;
			}
			return nullptr;
		}

		void UpdateEntities() {
			for (auto& pair : m_entities) {
				pair.second->Update();
			}
		}

		int GetNextID() {
			return m_nextID;
		}

		const std::unordered_map<int, BaseGameEntity*> GetAllEntities() const {
			return m_entities;
		}

	};
}
