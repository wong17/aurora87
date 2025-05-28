#include "LittleHealthEntity.h"

namespace AIEnviroment {
	LittleHealthEntity::LittleHealthEntity()
	{
		this->SetCantHeath(20.0f);
		m_tagged = false;
		m_position = glm::vec3(0.0f, 0.0, 0.0);
		m_CurrentStateLittle = nullptr;

	}
	LittleHealthEntity::~LittleHealthEntity()
	{
	}
	void LittleHealthEntity::Update()
	{
		if (m_CurrentStateLittle) {
			m_CurrentStateLittle->Execute(this);
		}
	}
	void LittleHealthEntity::ChangeFiniteStateLittle(FiniteStateHealthLittle* finiteStateLittle)
	{
		assert(m_CurrentStateLittle && finiteStateLittle);
		m_CurrentStateLittle->Exit(this);

		m_CurrentStateLittle = finiteStateLittle;
		m_CurrentStateLittle->Enter(this);
	}
	void LittleHealthEntity::HandleEvent(const EventData& eventData)
	{
	}
}