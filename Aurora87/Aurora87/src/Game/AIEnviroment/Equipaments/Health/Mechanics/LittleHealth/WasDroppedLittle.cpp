#include "WasDroppedLittle.h"
namespace AIEnviroment {
	WasDroppedLittle* WasDroppedLittle::Instance()
	{
		if (!m_instance) {
			m_instance = new WasDroppedLittle();
		}
		return m_instance;
	}
	void WasDroppedLittle::Enter(LittleHealthEntity* entity)
	{
	}
	void WasDroppedLittle::Execute(LittleHealthEntity* entity)
	{
	}
	void WasDroppedLittle::Exit(LittleHealthEntity* entity)
	{
	}
	WasDroppedLittle* WasDroppedLittle::m_instance = nullptr;
}