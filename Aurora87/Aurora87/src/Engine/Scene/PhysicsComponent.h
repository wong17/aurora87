#pragma once

#include <glm/glm.hpp>

namespace Engine
{
	struct PhysicsComponent
	{
		glm::vec3 Velocity{ 0.0f };
		glm::vec3 Acceleration{ 0.0f };
		float Mass = 1.0f;
		bool UseGravity = true;
	};
}