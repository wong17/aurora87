#include "Texture.h"

namespace Engine
{
	static GLint CalculateUnpackAlignment(uint32_t width, uint32_t bytesPerPixel) 
	{
		uint32_t rowBytes = width * bytesPerPixel;
		
		for (GLint a : {8, 4, 2, 1}) 
		{
			if ((rowBytes % a) == 0)
				return a;
		}

		return 1;
	}

	static GLenum ImageFormatToOpenGLDataFormat(ImageFormat format)
	{
		switch (format)
		{
			case ImageFormat::R8:      return GL_RED;
			case ImageFormat::RG8:     return GL_RG;
			case ImageFormat::RGB8:    return GL_RGB;
			case ImageFormat::RGBA8:   return GL_RGBA;
			case ImageFormat::RGBA32F: return GL_RGBA;
			case ImageFormat::SRGB8:   return GL_RGB;
			case ImageFormat::SRGBA8:  return GL_RGBA;
			default:                   return 0;
		}
	}

	static GLenum ImageFormatToOpenGLInternalFormat(ImageFormat format)
	{
		switch (format)
		{
			case ImageFormat::R8:        return GL_R8;
			case ImageFormat::RG8:       return GL_RG8;
			case ImageFormat::RGB8:      return GL_RGB8;
			case ImageFormat::RGBA8:     return GL_RGBA8;
			case ImageFormat::RGBA32F:   return GL_RGBA32F;
			case ImageFormat::SRGB8:     return GL_SRGB8;
			case ImageFormat::SRGBA8:    return GL_SRGB8_ALPHA8;
			case ImageFormat::BC1_RGB:   return GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
			case ImageFormat::BC3_RGBA:  return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			case ImageFormat::BC7:       return GL_COMPRESSED_RGBA_BPTC_UNORM;
			case ImageFormat::ETC2_RGB8: return GL_COMPRESSED_RGB8_ETC2;
			default:                     return 0;
		}
	}

	Texture::Texture(const TextureSpecification& specification)
		: m_IsLoaded(false), m_InternalFormat(0), m_DataFormat(0), m_RendererID(0)
	{
		m_Specification = specification;
		m_Width = m_Specification.Width;
		m_Height = m_Specification.Height;

		// Check valid dimensions
		if (m_Width == 0 || m_Height == 0) 
		{
			throw std::invalid_argument("Texture::Texture: Texture dimensions must be greater than 0.");
			return;
		}

		m_InternalFormat = ImageFormatToOpenGLInternalFormat(m_Specification.Format);
		m_DataFormat = ImageFormatToOpenGLDataFormat(m_Specification.Format);

		// Calculate mipmap levels ONLY if uncompressed
		if (!m_Specification.IsCompressed) {
			GLsizei calculatedLevels = m_Specification.GenerateMips	? Utils::CalculateMipLevels(m_Width, m_Height) : 1;
			m_Specification.MipLevels = calculatedLevels; // Update MipLevels
		}

		// Create texture using DSA
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		GLCall(glTextureStorage2D(m_RendererID, m_Specification.MipLevels, m_InternalFormat, m_Width, m_Height));

		ApplyTextureParameters();

		if (m_Specification.GenerateMips) 
		{
			glGenerateTextureMipmap(m_RendererID);
		}

		// Now you only have to pass the texture data through SetData
		m_IsLoaded = true;
	}

	Texture::Texture(const std::string& path) 
		: m_Path(path), m_Width(0), m_Height(0), m_IsLoaded(false), m_InternalFormat(0), m_DataFormat(0), 
		m_RendererID(0), m_Specification()
	{
		m_Name = Utils::ExtractFileName(path);
		LoadFromFile(path);
	}

	Texture::Texture(const TextureSpecification& specification, const std::string& path)
		: m_Path(path), m_Specification(specification), m_Width(0), m_Height(0), 
		m_IsLoaded(false), m_InternalFormat(0), m_DataFormat(0), m_RendererID(0)
	{
		m_Name = Utils::ExtractFileName(path);
		LoadFromFile(path);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void Texture::Bind(uint32_t slot) const
	{
		if (m_IsLoaded) 
		{
            glBindTextureUnit(slot, m_RendererID);
        }
	}

    void Texture::SetData(void* data, uint32_t size) const
    {
		uint32_t bpp = 0;
		switch (m_DataFormat) 
		{
			case GL_RED:        bpp = 1; break;   // 1 channel (8 bits)
			case GL_RG:         bpp = 2; break;   // 2 channels (8 bits each)
			case GL_RGB:        bpp = 3; break;   // 3 channels
			case GL_RGBA:       bpp = 4; break;   // 4 channels
			case GL_RGBA32F:    bpp = 16; break;  // 4 float components (4 bytes each)
			// For integer and other formats
			case GL_R16:        bpp = 2; break;   // 1 channel (16 bits)
			case GL_RG16:       bpp = 4; break;   // 2 channels (16 bits each)
			case GL_RGB16:      bpp = 6; break;   // 3 channels (16 bits each)
			case GL_RGBA16:     bpp = 8; break;   // 4 channels (16 bits each)
			default: 
				std::cerr << "Texture::SetData: Unsupported format: 0x" << std::hex << m_DataFormat << std::dec << "\n"; 
				return;
		}

		uint32_t expectedSize = m_Width * m_Height * bpp;
		if (size != expectedSize) 
		{
            throw std::runtime_error("Incorrect data size. Expected: " + std::to_string(expectedSize) 
				+ ", Received: " + std::to_string(size));
		}

		// Determine data type (based on internal format)
		GLenum type = GL_UNSIGNED_BYTE;
		if (m_InternalFormat == GL_RGBA32F || m_InternalFormat == GL_R32F)	type = GL_FLOAT;
		else if (m_InternalFormat == GL_R16 || m_InternalFormat == GL_RG16) type = GL_UNSIGNED_SHORT; // For 16-bit formats
		GLCall(glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, type, data));

		if (m_Specification.GenerateMips) 
		{
			glGenerateTextureMipmap(m_RendererID);
		}
    }

	void Texture::SetCompressedData(const void* data, size_t dataSize, uint32_t level) const
	{
		// Use real mipmap dimensions if available
		uint32_t w = std::max(1u, m_Width >> level);
		uint32_t h = std::max(1u, m_Height >> level);

		if (w == 0 || h == 0) 
		{
			throw std::runtime_error("Texture::SetCompressedData: Invalid mipmap dimensions: " 
				+ std::to_string(w) + "x" + std::to_string(h));
		}

		GLCall(glCompressedTextureSubImage2D(m_RendererID, level, 0, 0, w, h, m_InternalFormat, static_cast<GLsizei>(dataSize), data));
	}

	void Texture::LoadFromFile(const std::string& path)
	{
		stbi_set_flip_vertically_on_load(1);
		int width, height, channels;

		// Try loading HDR first
		float* hdrData = nullptr;
		if (stbi_is_hdr(path.c_str()))
		{
			hdrData = stbi_loadf(path.c_str(), &width, &height, &channels, 0);
		}
		// If HDR cannot be loaded, try to load LDR
		stbi_uc* ldrData = nullptr;
		if (!hdrData) 
		{
			ldrData = stbi_load(path.c_str(), &width, &height, &channels, 0);
			if (!ldrData) 
			{
				std::cerr << "Error loading texture " << path << "\n";
				return;
			}
		}
		
		// Update specifications
		m_Width = static_cast<uint32_t>(width);
		m_Height = static_cast<uint32_t>(height);
		m_Specification.Width = width;
		m_Specification.Height = height;

		// Automatically detect format if not specified, deduce from channels
		if (m_Specification.Format == ImageFormat::None) 
		{
			if (hdrData) 
			{
				m_Specification.Format = ImageFormat::RGBA32F;
				channels = 4;
			}
			else 
			{
				switch (channels) 
				{
					case 1: m_Specification.Format = ImageFormat::R8; break;
					case 2: m_Specification.Format = ImageFormat::RG8; break;
					case 3: m_Specification.Format = m_Specification.SRGB ? ImageFormat::SRGB8 : ImageFormat::RGB8; break;
					case 4: m_Specification.Format = m_Specification.SRGB ? ImageFormat::SRGBA8 : ImageFormat::RGBA8; break;
					default:
						stbi_image_free(ldrData);
						stbi_image_free(hdrData);
						std::cerr << "Texture::LoadFromFile: Number of channels not supported " << channels << " in " << path << "\n";
						return;
				}
			}
		}

		// Configuring OpenGL formats
		m_InternalFormat = ImageFormatToOpenGLInternalFormat(m_Specification.Format);
		m_DataFormat = ImageFormatToOpenGLDataFormat(m_Specification.Format);

		// Create texture using DSA
		GLsizei levels = m_Specification.GenerateMips ? Utils::CalculateMipLevels(m_Width, m_Height) : 1;
		m_Specification.MipLevels = static_cast<uint32_t>(levels);
		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID));
		GLCall(glTextureStorage2D(m_RendererID, levels, m_InternalFormat, m_Width, m_Height));

		uint32_t bpp = channels;
		GLint defaultAlign = 0;
		glGetIntegerv(GL_UNPACK_ALIGNMENT, &defaultAlign);

		GLint neededAlign = CalculateUnpackAlignment(m_Width, bpp);

		if (neededAlign != defaultAlign)
			glPixelStorei(GL_UNPACK_ALIGNMENT, neededAlign);

		// Upload data
		if (hdrData) 
		{
			GLCall(glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_FLOAT, hdrData));
			stbi_image_free(hdrData);
		}
		else 
		{
			GLCall(glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, ldrData));
			stbi_image_free(ldrData);
		}

		if (neededAlign != defaultAlign)
			glPixelStorei(GL_UNPACK_ALIGNMENT, defaultAlign);

		ApplyTextureParameters();

		// Determine if mipmaps are generated
		if (m_Specification.GenerateMips) 
		{
			glGenerateTextureMipmap(m_RendererID);
		}

		m_IsLoaded = true;
	}

	void Texture::ApplyTextureParameters() const
	{
		// This defines how the texture is sampled when reduced and when enlarged.
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, m_Specification.GenerateMips ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// This controls how coordinates outside the range [0,1] are handled.
		if (IsNPOT())
		{
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		else
		{
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, m_Specification.WrapS);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, m_Specification.WrapT);
		}
	}
}