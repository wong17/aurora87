#pragma once
#include "AIEnviroment/Base/BaseGameEntity.h"
#include "AIEnviroment/EventGame/EventData.h"
#include "AIEnviroment/EventGame/EventManager.h"

namespace AIEnviroment {
	class Health {
	private:
		float m_CantHealth = 0.0f;
		bool WasPicked = false;

	public:
		Health() {};
		~Health() = default;
		
		float GetCantHealth() const { return m_CantHealth; }
		void SetCantHeath(const float cant) { m_CantHealth = cant; }
		bool GetWasPicked() const { return WasPicked; }
		void SetWasPicked(const bool picked) { WasPicked = picked; }
	};
}