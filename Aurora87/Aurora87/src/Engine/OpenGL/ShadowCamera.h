#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	class ShadowCamera
	{
	public:
		virtual ~ShadowCamera() = default;
		virtual void SetLightParams(const glm::vec3& position, const glm::vec3& direction) = 0;
		virtual void UpdateMatrices() = 0;
		virtual void Resize(int width, int height) {}
		virtual const glm::mat4& GetLightSpaceMatrix() const = 0;
	};
}