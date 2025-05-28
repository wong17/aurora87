#include "WasDroppedNormal.h"
namespace AIEnviroment {
	WasDroppedNormal* WasDroppedNormal::Instance()
	{
		if (!m_instance) {
			m_instance = new WasDroppedNormal();
		}
		return m_instance;
	}
	void WasDroppedNormal::Enter(NormalHealthEntity* entity)
	{
	}
	void WasDroppedNormal::Execute(NormalHealthEntity* entity)
	{
	}
	void WasDroppedNormal::Exit(NormalHealthEntity* entity)
	{
	}
	WasDroppedNormal* WasDroppedNormal::m_instance = nullptr;
}