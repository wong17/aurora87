#pragma once

#include <glm/glm.hpp>

namespace Engine
{
	enum class ShapeType { AABB, Sphere, Mesh };

	struct CollisionComponent
	{
		ShapeType Type = ShapeType::Sphere;
		glm::vec3 HalfExtents{ 1.0f };
		float Radius = 1.0f;
	};
}