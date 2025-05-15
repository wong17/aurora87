#pragma once

#include "Engine/OpenGL/UniformBuffer.h"
#include "Engine/Scene/EntityManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstdint>

namespace Engine
{
	class SceneRenderer
	{
	public:
		SceneRenderer(UniformBuffer& cameraUniformBuffer)
			: m_CameraUniformBuffer(cameraUniformBuffer), m_View(1.0f), m_Projection(1.0f)
		{ }

		void SetViewProjection(const glm::mat4& view, const glm::mat4& projection);
		void RenderAll(const EntityManager& em, bool bindTextures = true);

	private:
		UniformBuffer& m_CameraUniformBuffer;
		glm::mat4 m_View, m_Projection;
	};
}