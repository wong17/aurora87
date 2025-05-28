#pragma once
#include "AIEnviroment/Equipaments/Health/Health.h"
#include "AIEnviroment/Base/BaseGameEntity.h"
#include "AIEnviroment/Base/GameEntityManager.h"
#include "AIEnviroment/EventGame/EventData.h"
#include "AIEnviroment/EventGame/EventManager.h"
#include "AIEnviroment/Equipaments/Health/Mechanics/BigHealth/FiniteStateHealthBig.h"
namespace AIEnviroment {
	class BigHealthEntity : public Health, public BaseGameEntity, public EventListener
	{
	private:
		FiniteStateHealthBig* m_CurrentStateBig;
		bool m_tagged;
		glm::vec3 m_position;
	public:
		BigHealthEntity();
		~BigHealthEntity();
		void Update();
		glm::vec3 GetPosition() const override { return m_position; }
		void ChangeFiniteStateNormal(FiniteStateHealthBig* finiteStateBig);

		std::string GetType() const override { return "ItemHealthLittle"; }
		EntityCategory GetCategory() const override { return EntityCategory::ITEM; }
		void HandleEvent(const EventData& eventData) override;
		double BRadius() const override { return 1.4f; }
		void Tag() override { m_tagged = true; }
		void UnTag() override { m_tagged = false; }
		bool IsTagged() const override { return m_tagged; }
	};
}