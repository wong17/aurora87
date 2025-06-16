#include "RectButton.h"

namespace Engine
{
	std::shared_ptr<Mesh> RectButton::GenerateMesh() const
	{
		std::vector<float> vertices = {
			// Position           // normal           // Texture coordinates
			-0.5f, 0.0f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
			 0.5f, 0.0f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
			 0.5f, 0.0f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
			-0.5f, 0.0f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
			 0.5f, 0.0f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
			-0.5f, 0.0f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f
		};

		std::vector<TextureData> textures;
		if (m_DiffuseTexture)
		{
			textures.push_back({
				MaterialTextureType::Diffuse,
				m_DiffuseTexture,
				MaterialTextureUniformName(MaterialTextureType::Diffuse),
				0,
				m_DiffuseTexture->GetSpecification().SRGB });
		}
		else if (!m_DiffuseTexturePath.empty())
		{
			TextureSpecification spec;
			spec.SRGB = true;

			if (auto tex = TextureManager::Get().Load(m_DiffuseTexturePath, spec))
				textures.push_back({ MaterialTextureType::Diffuse, tex, MaterialTextureUniformName(MaterialTextureType::Diffuse), 0, spec.SRGB });
			else
				std::cerr << "RectButton::GenerateMesh(): Error loading texture " << m_DiffuseTexturePath << "\n";
		}

		if (m_SpecularTexture)
		{
			textures.push_back({
				MaterialTextureType::Specular,
				m_SpecularTexture,
				MaterialTextureUniformName(MaterialTextureType::Specular),
				1,
				m_SpecularTexture->GetSpecification().SRGB });
		}
		else if (!m_SpecularTexturePath.empty())
		{
			if (auto tex = TextureManager::Get().Load(m_SpecularTexturePath))
				textures.push_back({ MaterialTextureType::Specular, tex, MaterialTextureUniformName(MaterialTextureType::Specular), 1, false });
			else
				std::cerr << "RectButton::GenerateMesh(): Error loading texture " << m_SpecularTexturePath << "\n";
		}

		return std::make_shared<Mesh>(vertices, textures);
	}
}