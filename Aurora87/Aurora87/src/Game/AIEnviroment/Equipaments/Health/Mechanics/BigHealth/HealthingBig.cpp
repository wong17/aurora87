#include "HealthingBig.h"
namespace AIEnviroment {
	HealthingBig* HealthingBig::Instance()
	{
		if (!m_instance) {
			m_instance = new HealthingBig();
		}
		return m_instance;
	}
	void HealthingBig::Enter(BigHealthEntity* Entity)
	{
	}
	void HealthingBig::Execute(BigHealthEntity* Entity)
	{
	}
	void HealthingBig::Exit(BigHealthEntity* Entity)
	{
	}
	HealthingBig* HealthingBig::m_instance = nullptr;
}