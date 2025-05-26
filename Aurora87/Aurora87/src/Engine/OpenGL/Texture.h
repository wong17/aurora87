#pragma once

#include "Engine/Util/Utils.h"
#include "Engine/Core/DebugGL.h"

#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include <iostream>
#include <cstdint>
#include <string>
#include <cmath>
#include <stdexcept>

namespace Engine
{
	enum class ImageFormat
	{
		None = 0,
		// Formatos no comprimidos
		R8,
		RG8,
		RGB8,
		RGBA8,
		RGBA32F,
		SRGB8,
		SRGBA8,
		// Formatos comprimidos
		BC1_RGB,       // DXT1
		BC1_SRGB,      // DXT1 sRGB
		BC3_RGBA,      // DXT5
		BC3_SRGBA,     // DXT5 sRGB
		BC7,           // BPTC
		ETC2_RGB8
	};

	struct TextureSpecification
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		ImageFormat Format = ImageFormat::None;
		bool GenerateMips = true;
		// Par�metros de wrapping
		GLenum WrapS = GL_REPEAT;
		GLenum WrapT = GL_REPEAT;
		bool SRGB = false;
		bool IsCompressed = false;
		uint32_t MipLevels = 1;
	};

	class Texture
	{
	public:
		Texture(const TextureSpecification& specification);
		Texture(const std::string& path);
		Texture(const TextureSpecification& specification, const std::string& path);
		~Texture();

		void Bind(uint32_t slot = 0) const;

		void SetData(void* data, uint32_t size) const;
		// Sube una textura comprimida para un nivel de mipmap
		void SetCompressedData(const void* data, size_t dataSize, uint32_t level) const;
		inline void SetName(const std::string& name) { m_Name = name; }
		// Esto es para ver si la textura es NPOT (Non Power of Two)
		inline bool IsNPOT() const { return (m_Width & (m_Width - 1)) != 0 || (m_Height & (m_Height - 1)) != 0; }

		const std::string& GetPath() const { return m_Path; }
		const std::string& GetName() const { return m_Name; }
		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }
		uint32_t GetRendererID() const { return m_RendererID; }
		bool IsLoaded() const { return m_IsLoaded; }
		const TextureSpecification& GetSpecification() const { return m_Specification; }

		bool operator==(const Texture& other) const
		{
			return m_RendererID == other.GetRendererID();
		}

	private:
		void LoadFromFile(const std::string& path);
		void ApplyTextureParameters() const;
	private:
		std::string m_Path, m_Name;
		uint32_t m_RendererID;
		uint32_t m_Width, m_Height;
		bool m_IsLoaded = false;
		GLenum m_InternalFormat, m_DataFormat;
		TextureSpecification m_Specification;
	};
}