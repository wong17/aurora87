#pragma once
#include "AIEnviroment/Equipaments/Guns/Bullets/BulletManager.h"
namespace AIEnviroment {
	enum class TypesCharger {
		NONE,
		AK_47,
	};
	class ChargersManager {
	private:
		int m_CantBullets = 0;
		bool WasPicked = false;
		bool IsFull = false;
		bool IsVoid = true;
		bool IsDropped = true;
	public:
		ChargersManager() {};
		~ChargersManager() = default;

		int GetCantHealth() const { return m_CantBullets; }
		void SetCantHeath(const int cant) { m_CantBullets = cant; }
		bool GetWasPicked() const { return WasPicked; }
		void SetWasPicked(const bool picked) { WasPicked = picked; }
	};
}