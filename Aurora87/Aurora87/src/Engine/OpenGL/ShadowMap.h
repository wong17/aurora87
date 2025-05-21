#pragma once

#include "Engine/Core/DebugGL.h"
#include "Engine/OpenGL/ShadowCamera.h"
#include "Engine/OpenGL/PerspectiveShadowCamera.h"
#include "Engine/OpenGL/OrthographicShadowCamera.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <stdexcept>
#include <string>
#include <iostream>

namespace Engine
{
	/*
	 * La proyección de perspectiva a menudo se usa para luces tipo spot y point, mientras que
	 * la proyección ortográfica se usa para luces direccionales.
	 */
	struct DepthTextureSpecification
	{
		GLenum MinFilter = GL_NEAREST;
		GLenum MagFilter = GL_NEAREST;
		GLenum WrapS = GL_CLAMP_TO_BORDER;
		GLenum WrapT = GL_CLAMP_TO_BORDER;
		glm::vec4 BorderColor{ 1.0f, 1.0f, 1.0f, 1.0f };
	};

	class ShadowMap
	{
	public:
		// Width y Height deberian ser multiplos de 1024
		ShadowMap(std::shared_ptr<ShadowCamera> camera,
			unsigned int width = 1024,
			unsigned int height = 1024,
			DepthTextureSpecification texSpec = {});
		~ShadowMap();

		void Resize(unsigned int newWidth, unsigned int newHeight);
		void BindDepthTexture(unsigned int slot = 0) const;
		void SetLightPositionAndDirection(const glm::vec3& position, const glm::vec3& direction);
		
		void BeginDepthPass();
		void EndDepthPass() const;

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }
		inline GLuint GetDepthTexture() const { return m_DepthTex; }
		inline const ShadowCamera& GetCamera() const { return *m_Camera; }
		inline const glm::mat4& GetLightSpaceMatrix() const { return m_Camera->GetLightSpaceMatrix(); }

	private:
		void InitFramebuffer();

		unsigned int m_Width, m_Height;
		GLuint m_FrameBufferId = 0;
		GLuint m_DepthTex = 0;

		std::shared_ptr<ShadowCamera> m_Camera;
		DepthTextureSpecification m_TexSpec;
		// Almacena el viejo viewport (x, y, w, h)
		GLint m_PrevViewport[4];
	};
}