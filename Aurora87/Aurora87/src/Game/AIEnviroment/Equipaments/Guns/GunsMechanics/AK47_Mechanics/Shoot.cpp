#include "Shoot.h"

namespace AIEnviroment 
{
	Shoot* AIEnviroment::Shoot::Instance()
	{
		if (!m_instance) {
			m_instance = new Shoot();
		}
		return m_instance;
	}

	void AIEnviroment::Shoot::Enter(AK47_Entity* entity)
	{
	}

	void AIEnviroment::Shoot::Execute(AK47_Entity* entity)
	{
	}

	void AIEnviroment::Shoot::Exit(AK47_Entity* entity)
	{
	}
	Shoot* Shoot::m_instance = nullptr;
}