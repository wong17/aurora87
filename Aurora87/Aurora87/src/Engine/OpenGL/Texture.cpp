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
			default:                     return 0;
		}
	}

	Texture::Texture(const TextureSpecification& specification)
		: m_Specification(specification), m_Width(m_Specification.Width), 
		m_Height(m_Specification.Height), m_BPP(0), m_IsLoaded(false), 
		m_InternalFormat(0), m_DataFormat(0), m_RendererID(0)
	{
		// Verificar dimensiones válidas
		if (m_Width == 0 || m_Height == 0) {
			throw std::invalid_argument("Dimensiones de la textura deben ser mayores a 0.");
			return;
		}

		m_InternalFormat = ImageFormatToOpenGLInternalFormat(m_Specification.Format);
		m_DataFormat = ImageFormatToOpenGLDataFormat(m_Specification.Format);

		// Crear la textura usando DSA
		GLsizei levels = m_Specification.GenerateMips ? Utils::CalculateMipLevels(m_Width, m_Height) : 1;
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		GLCall(glTextureStorage2D(m_RendererID, levels, m_InternalFormat, m_Width, m_Height));

		ApplyTextureParameters();

		if (m_Specification.GenerateMips) {
			glGenerateTextureMipmap(m_RendererID);
		}

		// Ahora solo debe de pasar los datos de la textura por SetData
		m_IsLoaded = true;
	}

	Texture::Texture(const std::string& path) 
		: m_Path(path), m_Width(0), m_Height(0), m_BPP(0), m_IsLoaded(false), m_InternalFormat(0), m_DataFormat(0), m_RendererID(0), m_Specification()
	{
		m_Name = Utils::ExtractFileName(path);
		LoadFromFile(path);
	}

	Texture::Texture(const TextureSpecification& specification, const std::string& path)
		: m_Path(path), m_Specification(specification), m_Width(0), m_Height(0), 
		m_BPP(0), m_IsLoaded(false), m_InternalFormat(0), m_DataFormat(0), m_RendererID(0)
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
		if (m_IsLoaded) {
            glBindTextureUnit(slot, m_RendererID);
        }
	}

    void Texture::SetData(void* data, uint32_t size) const
    {
		uint32_t bpp = 0;
		switch (m_DataFormat) {
			case GL_RED:		bpp = 1; break;
			case GL_RGB:		bpp = 3; break;
			case GL_RGBA:		bpp = 4; break;
			case GL_RGBA32F:	bpp = 16; break; // 4 componentes * 4 bytes
			default:
				std::cerr << "Formato no soportado" << std::endl;
				return;
		}

		uint32_t expectedSize = m_Width * m_Height * bpp;
		if (size != expectedSize) {
            throw std::runtime_error("Tamanio de datos incorrecto. Esperado: " + std::to_string(expectedSize) + ", Recibido: " + std::to_string(size));
		}

		GLenum type = (m_InternalFormat == GL_RGBA32F) ? GL_FLOAT : GL_UNSIGNED_BYTE;
		GLCall(glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, type, data));

		if (m_Specification.GenerateMips) {
			glGenerateTextureMipmap(m_RendererID);
		}
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