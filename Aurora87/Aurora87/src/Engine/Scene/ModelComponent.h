#pragma once

#include "Engine/OpenGL/Model.h"

#include <memory>
#include <cstdint>

namespace Engine
{
	struct ModelComponent
	{
		std::shared_ptr<Engine::Model> ModelPtr;
		uint32_t InstanceCount = 0;
	};
}