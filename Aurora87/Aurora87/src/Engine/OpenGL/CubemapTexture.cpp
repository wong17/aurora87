#include "CubemapTexture.h"

namespace Engine
{
	CubemapTexture::CubemapTexture(const std::vector<std::string>& faces, const CubemapTextureSpecification& spec)
		: m_Faces(faces), m_Width(0), m_Height(0), m_Channels(0), m_IsLoaded(false), m_RendererID(0), m_Specification(spec)
	{
		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID);
		LoadCubemap(faces);
	}

	CubemapTexture::~CubemapTexture()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void CubemapTexture::Bind(uint32_t slot) const
	{
		if (m_IsLoaded) {
			glBindTextureUnit(slot, m_RendererID);
		}
	}

	void CubemapTexture::LoadCubemap(const std::vector<std::string>& faces)
	{
		if (faces.size() != 6) 
		{
			throw std::runtime_error("CubemapTexture::LoadCubemap: Se requieren 6 caras para crear un cubemap.");
			return;
		}

		std::vector<unsigned char*> buffers(6);
		for (size_t i = 0; i < faces.size(); ++i) {
			int width, height, channels;
			stbi_set_flip_vertically_on_load(false);
			buffers[i] = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
			if (!buffers[i]) 
			{
				std::cerr << "CubemapTexture::LoadCubemap: Error al cargar cara de cubemap " << faces[i] << std::endl;
				for (auto ptr : buffers) 
					if (ptr) stbi_image_free(ptr);
				return;
			}

			if (i == 0) {
				m_Width = static_cast<uint32_t>(width);
				m_Height = static_cast<uint32_t>(height);
				m_Channels = static_cast<uint32_t>(channels);

				GLenum internalFmt = (channels == 4 ? GL_RGBA8 : GL_RGB8);
				GLsizei levels = m_Specification.GenerateMips ? Utils::CalculateMipLevels(m_Width, m_Height) : 1;
				GLCall(glTextureStorage2D(m_RendererID, m_Specification.GenerateMips ? levels : 1, internalFmt, m_Width, m_Height));
			}
		}

		// Subir datos de cada cara
		GLenum dataFmt = (m_Channels == 4 ? GL_RGBA : GL_RGB);
		for (GLint face = 0; face < 6; ++face) {
			GLCall(glTextureSubImage3D(
				m_RendererID, 
				0,					// mipmap level
				0,					// x offset
				0,					// y offset	
				face,				// z offset	
				m_Width, m_Height,	// width, height 
				1,					// depth
				dataFmt,			// data format
				GL_UNSIGNED_BYTE,	// data type
				buffers[face]		// data
			));
			stbi_image_free(buffers[face]);
		}

		ApplyTextureParameters();

		if (m_Specification.GenerateMips) {
			glGenerateTextureMipmap(m_RendererID);
		}

		m_IsLoaded = true;
	}

	void CubemapTexture::ApplyTextureParameters() const
	{
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, m_Specification.WrapS);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, m_Specification.WrapT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, m_Specification.WrapR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, m_Specification.GenerateMips ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, m_Specification.MagFilter);
	}
}
