#include "HealthingNormal.h"
namespace AIEnviroment {
	HealthingNormal* HealthingNormal::Instance()
	{
		if (!m_instance) {
			m_instance = new HealthingNormal();
		}
		return m_instance;
	}
	void HealthingNormal::Enter(NormalHealthEntity* Entity)
	{
	}
	void HealthingNormal::Execute(NormalHealthEntity* Entity)  
	{
	}
	void HealthingNormal::Exit(NormalHealthEntity* Entity)
	{
	}
	HealthingNormal* HealthingNormal::m_instance = nullptr;
}