#pragma once
#include <vector>
#include <AIEnviroment/Base/BaseGameEntity.h>
#include "AIEnviroment/EventGame/EventData.h"
#include <AIEnviroment/EventGame/EventManager.h>
#include <AIEnviroment/Equipaments/Guns/GunsMechanics/FiniteStateAK47.h>
#include <AIEnviroment/Equipaments/Guns/Bullets/BulletManager.h>
#include <AIEnviroment/Equipaments/Guns/GunsManager.h>
namespace AIEnviroment {
	class AK47_Entity : public BaseGameEntity, public EventListener, public GunsManager
	{
	private:
		FiniteStateAK7* m_CurrentStateAK7;
	public:
		AK47_Entity();
		~AK47_Entity();
		void Update();
		void ChangeFiniteStateAK7(FiniteStateAK7* finiteStateAK7);
		std::string GetType() const override { return "AK-47"; }
		void HandleEvent(const EventData& eventData) override;
		EntityCategory GetCategory() const override { return EntityCategory::ITEM; }

	};
}