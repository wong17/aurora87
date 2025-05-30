#include "Idle.h"
#include "AIEnviroment/Equipaments/Guns/AK47_Entity.h"
namespace AIEnviroment {
	Idle* Idle::Instance()
	{
		if (!m_instance) {
			m_instance = new Idle();
		}
		return m_instance;
	}

	void Idle::Enter(AK47_Entity* entity)
	{
	}

	void Idle::Execute(AK47_Entity* entity)
	{
	}

	void Idle::Exit(AK47_Entity* entity)
	{
	}
	void Idle::HandleEvent(const EventData& eventData)
	{
		if (eventData.type == EventType::CHARGING_AK47) {

		}
	}
	Idle* Idle::m_instance = nullptr;

}