#include "Texture.h"

namespace Engine
{
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

		// Verificar dimensiones válidas
		if (m_Width == 0 || m_Height == 0) 
		{
			throw std::invalid_argument("Texture::Texture: Dimensiones de la textura deben ser mayores a 0.");
			return;
		}

		m_InternalFormat = ImageFormatToOpenGLInternalFormat(m_Specification.Format);
		m_DataFormat = ImageFormatToOpenGLDataFormat(m_Specification.Format);

		// Calcular niveles de mipmap SOLO si no es comprimida
		if (!m_Specification.IsCompressed) {
			GLsizei calculatedLevels = m_Specification.GenerateMips	? Utils::CalculateMipLevels(m_Width, m_Height) : 1;
			m_Specification.MipLevels = calculatedLevels; // Actualizar MipLevels
		}

		// Crear la textura usando DSA
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		GLCall(glTextureStorage2D(m_RendererID, m_Specification.MipLevels, m_InternalFormat, m_Width, m_Height));

		ApplyTextureParameters();

		if (m_Specification.GenerateMips) 
		{
			glGenerateTextureMipmap(m_RendererID);
		}

		// Ahora solo debe de pasar los datos de la textura por SetData
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
			case GL_RED:        bpp = 1; break;   // 1 canal (8 bits)
			case GL_RG:         bpp = 2; break;   // 2 canales (8 bits cada uno)
			case GL_RGB:        bpp = 3; break;   // 3 canales
			case GL_RGBA:       bpp = 4; break;   // 4 canales
			case GL_RGBA32F:    bpp = 16; break;  // 4 componentes float (4 bytes cada una)
			// Para formatos enteros y otros
			case GL_R16:        bpp = 2; break;   // 1 canal (16 bits)
			case GL_RG16:       bpp = 4; break;   // 2 canales (16 bits cada uno)
			case GL_RGB16:      bpp = 6; break;   // 3 canales (16 bits cada uno)
			case GL_RGBA16:     bpp = 8; break;   // 4 canales (16 bits cada uno)
			default: 
				std::cerr << "Texture::SetData: Formato no soportado: 0x" << std::hex << m_DataFormat << std::dec << "\n"; 
				return;
		}

		uint32_t expectedSize = m_Width * m_Height * bpp;
		if (size != expectedSize) 
		{
            throw std::runtime_error("Tamanio de datos incorrecto. Esperado: " + std::to_string(expectedSize) 
				+ ", Recibido: " + std::to_string(size));
		}

		// Determinamos tipo de dato (basado en formato interno)
		GLenum type = GL_UNSIGNED_BYTE;
		if (m_InternalFormat == GL_RGBA32F || m_InternalFormat == GL_R32F)	type = GL_FLOAT;
		else if (m_InternalFormat == GL_R16 || m_InternalFormat == GL_RG16) type = GL_UNSIGNED_SHORT; // Para formatos de 16 bits
		GLCall(glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, type, data));

		if (m_Specification.GenerateMips) 
		{
			glGenerateTextureMipmap(m_RendererID);
		}
    }

	void Texture::SetCompressedData(const void* data, size_t dataSize, uint32_t level) const
	{
		// Usar dimensiones reales del mipmap si están disponibles
		uint32_t w = std::max(1u, m_Width >> level);
		uint32_t h = std::max(1u, m_Height >> level);

		if (w == 0 || h == 0) 
		{
			throw std::runtime_error("Texture::SetCompressedData: Dimensiones de mipmap inválidas: " 
				+ std::to_string(w) + "x" + std::to_string(h));
		}

		GLCall(glCompressedTextureSubImage2D(m_RendererID, level, 0, 0, w, h, m_InternalFormat, static_cast<GLsizei>(dataSize), data));
	}

	void Texture::LoadFromFile(const std::string& path)
	{
		stbi_set_flip_vertically_on_load(1);
		int width, height, channels;

		// Intenta cargar HDR primero
		float* hdrData = nullptr;
		if (stbi_is_hdr(path.c_str()))
		{
			hdrData = stbi_loadf(path.c_str(), &width, &height, &channels, 0);
		}
		// Si no se puede cargar HDR, intenta cargar LDR
		stbi_uc* ldrData = nullptr;
		if (!hdrData) {
			ldrData = stbi_load(path.c_str(), &width, &height, &channels, 0);
			if (!ldrData) {
				std::cerr << "Error al cargar la textura " << path << "\n";
				return;
			}
		}
		
		// Actualizar especificaciones
		m_Width = width;
		m_Height = height;
		m_Specification.Width = width;
		m_Specification.Height = height;

		// Detectar formato automáticamente sino está especificado, deducirlo de los canales
		if (m_Specification.Format == ImageFormat::None) {
			if (hdrData) {
				m_Specification.Format = ImageFormat::RGBA32F;
				channels = 4;
			}
			else {
				switch (channels) {
				case 1: m_Specification.Format = ImageFormat::R8; break;
				case 2: m_Specification.Format = ImageFormat::RG8; break;
				case 3: m_Specification.Format = m_Specification.SRGB ? ImageFormat::SRGB8 : ImageFormat::RGB8; break;
				case 4: m_Specification.Format = m_Specification.SRGB ? ImageFormat::SRGBA8 : ImageFormat::RGBA8; break;
				default:
					std::cerr << "Error: Cantidad de canales no soportado " << channels << " en " << path << "\n";
					return;
				}
			}
		}

		// Configurar formatos OpenGL
		m_InternalFormat = ImageFormatToOpenGLInternalFormat(m_Specification.Format);
		m_DataFormat = ImageFormatToOpenGLDataFormat(m_Specification.Format);

		// Crear la textura usando DSA
		GLsizei levels = m_Specification.GenerateMips ? Utils::CalculateMipLevels(m_Width, m_Height) : 1;
		m_Specification.MipLevels = static_cast<int>(levels);
		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID));
		GLCall(glTextureStorage2D(m_RendererID, levels, m_InternalFormat, m_Width, m_Height));

		// Subir datos
		if (hdrData) {
			GLCall(glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_FLOAT, hdrData));
			stbi_image_free(hdrData);
		}
		else {
			GLCall(glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, ldrData));
			stbi_image_free(ldrData);
		}

		ApplyTextureParameters();

		// Determinar si se generan mipmaps
		if (m_Specification.GenerateMips) {
			glGenerateTextureMipmap(m_RendererID);
		}

		m_IsLoaded = true;
	}

	void Texture::ApplyTextureParameters() const
	{
		// Esto define cómo se muestrea la textura cuando se reduce y cuando se amplía
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, m_Specification.GenerateMips ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Esto controla cómo se manejan las coordenadas fuera del rango [0,1].
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