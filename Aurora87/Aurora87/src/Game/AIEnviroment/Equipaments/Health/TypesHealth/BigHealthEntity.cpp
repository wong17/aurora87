#include "BigHealthEntity.h"
namespace AIEnviroment {
	BigHealthEntity::BigHealthEntity()
	{
		this->SetCantHeath(100);
		m_tagged = false;
		m_position = glm::vec3(0.0f, 0.0f, 0.0f);
		m_CurrentStateBig = nullptr;
;	}
	BigHealthEntity::~BigHealthEntity()
	{
	}
	void BigHealthEntity::Update()
	{
		if (m_CurrentStateBig) {
			m_CurrentStateBig->Execute(this);
		}
	}
	void BigHealthEntity::ChangeFiniteStateNormal(FiniteStateHealthBig* finiteStateBig)
	{
		assert(m_CurrentStateBig && finiteStateBig);
		m_CurrentStateBig->Exit(this);

		m_CurrentStateBig = finiteStateBig;
		m_CurrentStateBig->Enter(this);
	}
	void BigHealthEntity::HandleEvent(const EventData& eventData)
	{
	}
}