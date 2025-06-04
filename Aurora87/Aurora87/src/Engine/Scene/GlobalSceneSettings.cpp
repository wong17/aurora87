#include "GlobalSceneSettings.h"

namespace Engine
{
	std::atomic<bool> g_EnableShadows = false;

	LightCounts g_LightCounts = { 0, 0, 0 };
}