#include "NormalHealthEntity.h"

namespace AIEnviroment {
	NormalHealthEntity::NormalHealthEntity()
	{
		this->SetCantHeath(50.0f);
		m_tagged = false;
		m_position = glm::vec3(0.0f, 0.0f, 0.0f);
		m_currentStateNormal = nullptr;
	}
	NormalHealthEntity::~NormalHealthEntity()
	{
	}
	void NormalHealthEntity::Update()
	{
		if (m_currentStateNormal) {
			m_currentStateNormal->Execute(this);
		}
	}
	void NormalHealthEntity::ChangeFiniteStateNormal(FiniteStateHealthNormal* finiteStateNormal)
	{
		assert(m_currentStateNormal && finiteStateNormal);
		m_currentStateNormal->Exit(this);

		m_currentStateNormal = finiteStateNormal;
		m_currentStateNormal->Enter(this);
	}
	void NormalHealthEntity::HandleEvent(const EventData& eventData)
	{
	}
}