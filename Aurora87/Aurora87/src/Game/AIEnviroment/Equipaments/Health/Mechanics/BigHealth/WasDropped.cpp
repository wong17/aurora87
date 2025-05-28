#include "WasDropped.h"
namespace AIEnviroment{
	WasDropped* WasDropped::Instance()
	{
		if (!m_instance) {
			m_instance = new WasDropped();
		}
		return m_instance;
	}
	void WasDropped::Enter(BigHealthEntity* entity)
	{
	}
	void WasDropped::Execute(BigHealthEntity* entity)
	{
	}
	void WasDropped::Exit(BigHealthEntity* entity)
	{
	}
	WasDropped* WasDropped::m_instance = nullptr;
}