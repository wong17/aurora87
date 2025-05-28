#pragma once
#include "AIEnviroment/Equipaments/Health/Health.h"
#include "AIEnviroment/Base/BaseGameEntity.h"
#include "AIEnviroment/Base/GameEntityManager.h"
#include "AIEnviroment/EventGame/EventData.h"
#include "AIEnviroment/EventGame/EventManager.h"
#include <AIEnviroment/Equipaments/Health/Mechanics/LittleHealth/FiniteStateHealthLittle.h>

namespace AIEnviroment {
	class LittleHealthEntity : public Health, public BaseGameEntity, public EventListener
	{
	private:
		FiniteStateHealthLittle* m_CurrentStateLittle;
		bool m_tagged;
		glm::vec3 m_position;
	public:
		LittleHealthEntity();
		~LittleHealthEntity();
		void Update();
		glm::vec3 GetPosition() const override { return m_position; }
		void ChangeFiniteStateLittle(FiniteStateHealthLittle* finiteStateLittle);

		std::string GetType() const override { return "ItemHealthLittle"; }
		EntityCategory GetCategory() const override { return EntityCategory::ITEM; }
		void HandleEvent(const EventData& eventData) override;
		double BRadius() const override { return 1.4f; }
		void Tag() override { m_tagged = true; }
		void UnTag() override { m_tagged = false; }
		bool IsTagged() const override { return m_tagged; }
	};

}