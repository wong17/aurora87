#include "SceneRenderer.h"

namespace Engine
{
	void SceneRenderer::SetViewProjection(const glm::mat4& view, const glm::mat4& projection)
	{
		m_View = view;
		m_Projection = projection;

		m_CameraUniformBuffer.Bind();
		m_CameraUniformBuffer.SetData("u_ViewMatrix", glm::value_ptr(view));
		m_CameraUniformBuffer.SetData("u_ProjectionMatrix", glm::value_ptr(projection));
	}

	void SceneRenderer::RenderDepth(const EntityManager& em, Shader& depthShader)
	{
		for (const auto& rec : em.GetRecords())
		{
			if (rec.instanceCount == 0)
				rec.entity->DrawDepth(depthShader);
			else
				rec.entity->DrawInstancedDepth(depthShader, rec.instanceCount);
		}
	}

	void SceneRenderer::RenderAll(const EntityManager& em, bool bindTextures)
	{
		for (const auto& rec : em.GetRecords())
		{
			if (rec.instanceCount <= 0)
				rec.entity->Draw(bindTextures, em.GetGlobalUniformBuffer(), rec.uniformBufferIndex);
			else
				rec.entity->DrawInstanced(bindTextures, em.GetGlobalUniformBuffer(), rec.uniformBufferIndex, rec.instanceCount);
		}
	}
}