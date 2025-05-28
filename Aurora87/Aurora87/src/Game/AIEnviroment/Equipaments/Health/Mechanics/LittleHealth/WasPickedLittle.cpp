#include "WasPickedLittle.h"
namespace AIEnviroment {
    WasPickedLittle* WasPickedLittle::Instance()
    {
        if (!m_instance) {
            m_instance = new WasPickedLittle();
        }
        return m_instance;
    }
    void WasPickedLittle::Enter(LittleHealthEntity* entity)
    {
    }
    void WasPickedLittle::Execute(LittleHealthEntity* entity)
    {
    }
    void WasPickedLittle::Exit(LittleHealthEntity* entity)
    {
    }
    WasPickedLittle* WasPickedLittle::m_instance = nullptr;
}