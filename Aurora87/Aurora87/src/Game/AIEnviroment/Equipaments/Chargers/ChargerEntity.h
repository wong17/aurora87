#pragma once
#include <AIEnviroment/Equipaments/Chargers/ChargersManager.h>
#include <AIEnviroment/Base/BaseGameEntity.h>
#include "AIEnviroment/EventGame/EventData.h"
#include <AIEnviroment/EventGame/EventManager.h>
namespace AIEnviroment {
	class ChargerEntity : public BaseGameEntity, public ChargersManager, public EventListener
	{
	private:
		std::vector<BulletManager*> m_Bullets;
		TypesCharger m_TypesChager;
	
	public:
		ChargerEntity();
		~ChargerEntity();
		void SetBullets(BulletManager* bullet) { m_Bullets.push_back(bullet); }
		std::vector<BulletManager*> GetCharger() const { return m_Bullets; }
		std::string GetType() const override { return "ChargerBullets"; }
		void HandleEvent(const EventData& eventData) override;
		EntityCategory GetCategory() const override { return EntityCategory::ITEM; }
		void SetTypeCharger(const TypesCharger type) { m_TypesChager = type; }
		TypesCharger GetTypesCharger() const { return m_TypesChager; }
	};
}