#include "ShadowMap.h"

namespace Engine
{
	ShadowMap::ShadowMap(std::shared_ptr<ShadowCamera> camera,
		unsigned int width, unsigned int height, DepthTextureSpecification texSpec)
		: m_Camera(camera), m_Width(width), m_Height(height), m_TexSpec(texSpec)
	{
		if (width == 0 || height == 0) 
		{
			throw std::invalid_argument("ShadowMap::ShadowMap: width and/or height cannot be equal to 0");
		}

		InitFramebuffer();
	}

	ShadowMap::~ShadowMap()
	{
		glDeleteFramebuffers(1, &m_FrameBufferId);
		glDeleteTextures(1, &m_DepthTex);
	}

	void ShadowMap::BeginDepthPass()
	{
		// Save current viewport
		glGetIntegerv(GL_VIEWPORT, m_PrevViewport);

		// Set viewport to shadow map size
		glViewport(0, 0, m_Width, m_Height);

		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferId);
		glClear(GL_DEPTH_BUFFER_BIT);

		m_Camera->UpdateMatrices();
	}

	void ShadowMap::EndDepthPass() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// restore the previous viewport
		glViewport(m_PrevViewport[0], m_PrevViewport[1], m_PrevViewport[2], m_PrevViewport[3]);
	}

	void ShadowMap::BindDepthTexture(unsigned int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_DepthTex);
	}

	void ShadowMap::Resize(unsigned int newWidth, unsigned int newHeight)
	{
		if (newWidth == m_Width && newHeight == m_Height)
			return;

		m_Width = newWidth;
		m_Height = newHeight;

		// Delete existing resources
		glDeleteFramebuffers(1, &m_FrameBufferId);
		glDeleteTextures(1, &m_DepthTex);

		// Re-create them with the new size
		InitFramebuffer();

		// Update aspect ratio if perspective camera
		if (auto* perspectiveCam = dynamic_cast<PerspectiveShadowCamera*>(m_Camera.get())) 
		{
			perspectiveCam->Resize(m_Width, m_Height);
		}
		else if (auto* orthoCam = dynamic_cast<OrthographicShadowCamera*>(m_Camera.get())) 
		{
			orthoCam->SetProjection(
				-static_cast<float>(m_Width / 2), static_cast<float>(m_Width / 2),
				-static_cast<float>(m_Height / 2), static_cast<float>(m_Height / 2));
		}
	}

	void ShadowMap::SetLightPositionAndDirection(const glm::vec3& position, const glm::vec3& direction)
	{
		m_Camera->SetLightParams(position, direction);
		m_Camera->UpdateMatrices();
	}

	void ShadowMap::InitFramebuffer()
	{
		// create texture for DepthBuffer
		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthTex);
		GLCall(glTextureStorage2D(m_DepthTex, 1, GL_DEPTH_COMPONENT24, m_Width, m_Height));
		// configure texture parameters
		GLCall(glTextureParameteri(m_DepthTex, GL_TEXTURE_MIN_FILTER, m_TexSpec.MinFilter));
		GLCall(glTextureParameteri(m_DepthTex, GL_TEXTURE_MAG_FILTER, m_TexSpec.MagFilter));
		GLCall(glTextureParameteri(m_DepthTex, GL_TEXTURE_WRAP_S, m_TexSpec.WrapS));
		GLCall(glTextureParameteri(m_DepthTex, GL_TEXTURE_WRAP_T, m_TexSpec.WrapT));
		GLCall(glTextureParameterfv(m_DepthTex, GL_TEXTURE_BORDER_COLOR, &m_TexSpec.BorderColor.x));

		// create rrameBuffer
		glCreateFramebuffers(1, &m_FrameBufferId);
		GLCall(glNamedFramebufferTexture(m_FrameBufferId, GL_DEPTH_ATTACHMENT, m_DepthTex, 0));
		// disable color buffer
		GLCall(glNamedFramebufferDrawBuffer(m_FrameBufferId, GL_NONE));
		GLCall(glNamedFramebufferReadBuffer(m_FrameBufferId, GL_NONE));

		GLenum status = glCheckNamedFramebufferStatus(m_FrameBufferId, GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cerr << "ShadowMap::InitFramebuffer: [Framebuffer Error] (" << status << "): "	
				<< FramebufferStatusToString(status) << std::endl;
			throw std::runtime_error("ShadowMap::InitFramebuffer: incomplete framebuffer");
		}
	}
}