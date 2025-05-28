#include "WasPicked.h"
namespace AIEnviroment {
    WasPicked* WasPicked::Instance()
    {
        if (!m_instance) {
            m_instance = new WasPicked();
        }
        return m_instance;
    }
    void WasPicked::Enter(BigHealthEntity* entity)
    {
    }
    void WasPicked::Execute(BigHealthEntity* entity)
    {
    }
    void WasPicked::Exit(BigHealthEntity* entity)
    {
    }
    WasPicked* WasPicked::m_instance = nullptr;
}