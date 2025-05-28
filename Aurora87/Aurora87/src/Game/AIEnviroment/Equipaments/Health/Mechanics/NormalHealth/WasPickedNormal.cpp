#include "WasPickedNormal.h"
namespace AIEnviroment {
    WasPickedNormal* WasPickedNormal::Instance()
    {
        if (!m_instance) {
            m_instance = new WasPickedNormal();
        }
        return m_instance;
    }
    void WasPickedNormal::Enter(NormalHealthEntity* entity)
    {
    }
    void WasPickedNormal::Execute(NormalHealthEntity* entity)
    {
    }
    void WasPickedNormal::Exit(NormalHealthEntity* entity)
    {
    }
    WasPickedNormal* WasPickedNormal::m_instance = nullptr;
}