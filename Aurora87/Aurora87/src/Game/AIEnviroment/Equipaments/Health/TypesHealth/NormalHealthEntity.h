#pragma once
#include "AIEnviroment/Equipaments/Health/Health.h"
#include "AIEnviroment/Base/BaseGameEntity.h"
#include "AIEnviroment/Base/GameEntityManager.h"
#include "AIEnviroment/EventGame/EventData.h"
#include "AIEnviroment/EventGame/EventManager.h"
#include <AIEnviroment/Equipaments/Health/Mechanics/NormalHealth/FiniteStateHealthNormal.h>
namespace AIEnviroment {
	class NormalHealthEntity : public Health, public BaseGameEntity, public EventListener
	{
	private:
		FiniteStateHealthNormal* m_currentStateNormal;
		bool m_tagged;
		glm::vec3 m_position;
	public:
		NormalHealthEntity();
		~NormalHealthEntity();
		void Update();
		glm::vec3 GetPosition() const override { return m_position; }
		void ChangeFiniteStateNormal(FiniteStateHealthNormal* finiteStateNormal);

		std::string GetType() const override { return "ItemHealthNormal"; }
		EntityCategory GetCategory() const override { return EntityCategory::ITEM; }
		void HandleEvent(const EventData& eventData) override;
		double BRadius() const override { return 1.4f; }
		void Tag() override { m_tagged = true; }
		void UnTag() override { m_tagged = false; }
		bool IsTagged() const override { return m_tagged; }
	};
}
