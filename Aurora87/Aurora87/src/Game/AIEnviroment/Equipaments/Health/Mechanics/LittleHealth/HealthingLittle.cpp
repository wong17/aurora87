#include "HealthingLittle.h"
namespace AIEnviroment {
	HealthingLittle* HealthingLittle::Instance()
	{
		if (!m_instance) {
			m_instance = new HealthingLittle();
		}
		return m_instance;
	}
	void HealthingLittle::Enter(LittleHealthEntity* Entity)
	{
	}
	void HealthingLittle::Execute(LittleHealthEntity* Entity)
	{
	}
	void HealthingLittle::Exit(LittleHealthEntity* Entity)
	{
	}
	HealthingLittle* HealthingLittle::m_instance = nullptr;
}