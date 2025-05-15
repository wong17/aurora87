#pragma once

#include "Engine/Util/Utils.h"
#include "Engine/Core/DebugGL.h"

#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <stdexcept>

namespace Engine
{
	struct CubemapTextureSpecification
	{
		bool GenerateMips = false;
		GLenum WrapS = GL_CLAMP_TO_EDGE;
		GLenum WrapT = GL_CLAMP_TO_EDGE;
		GLenum WrapR = GL_CLAMP_TO_EDGE;
		GLenum MinFilter = GL_LINEAR;
		GLenum MagFilter = GL_LINEAR;
	};

	class CubemapTexture
	{
	public:
		CubemapTexture(const std::vector<std::string>& faces, const CubemapTextureSpecification& spec = {});
		~CubemapTexture();

		void Bind(uint32_t slot = 0) const;

		uint32_t GetRendererID() const { return m_RendererID; }
		const std::string& GetName() const { return m_Name; }
		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }
		uint32_t GetChannels() const { return m_Channels; }
		const std::vector<std::string>& GetFaces() const { return m_Faces; }
		const CubemapTextureSpecification& GetSpecification() const { return m_Specification; }
		bool IsLoaded() const { return m_IsLoaded; }

		bool operator==(const CubemapTexture& other) const
		{
			return m_RendererID == other.GetRendererID();
		}

	private:
		void LoadCubemap(const std::vector<std::string>& faces);
		void ApplyTextureParameters() const;
	private:
		uint32_t m_RendererID;
		std::string m_Name;
		uint32_t m_Width = 0, m_Height = 0, m_Channels = 0;
		std::vector<std::string> m_Faces;
		CubemapTextureSpecification m_Specification;
		bool m_IsLoaded = false;
	};
}