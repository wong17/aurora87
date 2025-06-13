#include "Geometrics.h"

namespace Collision 
{

	AABBXZ  Collision::GeometricsBox::Calculate_AABBXZ(std::shared_ptr<Engine::Mesh> vertexModel) noexcept
	{
		float min_x = std::numeric_limits<float>::max();
		float max_x = std::numeric_limits<float>::min();
		float min_z = std::numeric_limits<float>::max();
		float max_z = std::numeric_limits<float>::min();
		for (const auto& vertex : vertexModel->GetVertices())
		{
			min_x = std::min(min_x, vertex.Position.x);
			max_x = std::max(max_x, vertex.Position.x);
			min_z = std::min(min_z, vertex.Position.z);
			max_z = std::max(max_z, vertex.Position.z);
		}
		return AABBXZ(min_x, max_x, min_z, max_z);
	}

	AABB Collision::GeometricsBox::Calculate_AABB(std::shared_ptr<Engine::Mesh> vertexModel) noexcept
    {
		glm::vec3 min = std::numeric_limits<glm::vec3>::max();
		glm::vec3 max = std::numeric_limits<glm::vec3>::min();
		for (const auto& vertex : vertexModel->GetVertices())
		{
			min.x = std::min(min.x, vertex.Position.x);
			min.y = std::min(min.y, vertex.Position.y);
			min.z = std::min(min.z, vertex.Position.z);

			max.x = std::max(max.x, vertex.Position.x);
			max.y = std::max(max.y, vertex.Position.y);
			max.z = std::max(max.z, vertex.Position.z);
		}
		return AABB(min, max);
    }
}