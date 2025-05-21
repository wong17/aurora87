#include "Pentagon.h"

namespace Engine
{
	std::shared_ptr<Mesh> Pentagon::GenerateMesh() const
	{
        std::vector<float> vertices = {
			// Posición                         // normales     // Coordenadas de textura
                  0.0f, 0.0f,       0.0f,      -0.0f, -1.0f,   -0.0f,        0.5f,      0.5f,
            -0.475528f, 0.0f, -0.154509f,      -0.0f, -1.0f,   -0.0f,   0.024472f, 0.654508f,
                  0.0f, 0.0f,      -0.5f,      -0.0f, -1.0f,   -0.0f,        0.5f,      1.0f,
                  0.0f, 0.0f,       0.0f,      -0.0f, -1.0f,   -0.0f,        0.5f,      0.5f,
            -0.293893f, 0.0f,  0.404509f,      -0.0f, -1.0f,   -0.0f,   0.206107f, 0.095491f,
            -0.475528f, 0.0f, -0.154509f,      -0.0f, -1.0f,   -0.0f,   0.024472f, 0.654508f,
                  0.0f, 0.0f,       0.0f,      -0.0f, -1.0f,   -0.0f,        0.5f,      0.5f,
             0.293893f, 0.0f,  0.404509f,      -0.0f, -1.0f,   -0.0f,   0.793893f, 0.095491f,
            -0.293893f, 0.0f,  0.404509f,      -0.0f, -1.0f,   -0.0f,   0.206107f, 0.095491f,
                  0.0f, 0.0f,       0.0f,      -0.0f, -1.0f,   -0.0f,        0.5f,      0.5f,
             0.475528f, 0.0f, -0.154509f,      -0.0f, -1.0f,   -0.0f,   0.975528f, 0.654508f,
             0.293893f, 0.0f,  0.404509f,      -0.0f, -1.0f,   -0.0f,   0.793893f, 0.095491f,
                  0.0f, 0.0f,       0.0f,      -0.0f, -1.0f,   -0.0f,        0.5f,      0.5f,
                  0.0f, 0.0f,      -0.5f,      -0.0f, -1.0f,   -0.0f,        0.5f,      1.0f,
             0.475528f, 0.0f, -0.154509f,      -0.0f, -1.0f,   -0.0f,   0.975528f, 0.654508f,
                  0.0f, 0.5f,       0.0f,      -0.0f,  1.0f,   -0.0f,        0.5f,      0.5f,
                  0.0f, 0.5f,      -0.5f,      -0.0f,  1.0f,   -0.0f,        0.5f,      1.0f,
            -0.475528f, 0.5f, -0.154509f,      -0.0f,  1.0f,   -0.0f,   0.024472f, 0.654508f,
                  0.0f, 0.5f,       0.0f,      -0.0f,  1.0f,   -0.0f,        0.5f,      0.5f,
            -0.475528f, 0.5f, -0.154509f,      -0.0f,  1.0f,   -0.0f,   0.024472f, 0.654508f,
            -0.293893f, 0.5f,  0.404509f,      -0.0f,  1.0f,   -0.0f,   0.206107f, 0.095491f,
                  0.0f, 0.5f,       0.0f,      -0.0f,  1.0f,   -0.0f,        0.5f,      0.5f,
            -0.293893f, 0.5f,  0.404509f,      -0.0f,  1.0f,   -0.0f,   0.206107f, 0.095491f,
             0.293893f, 0.5f,  0.404509f,      -0.0f,  1.0f,   -0.0f,   0.793893f, 0.095491f,
                  0.0f, 0.5f,       0.0f,      -0.0f,  1.0f,   -0.0f,        0.5f,      0.5f,
             0.293893f, 0.5f,  0.404509f,      -0.0f,  1.0f,   -0.0f,   0.793893f, 0.095491f,
             0.475528f, 0.5f, -0.154509f,      -0.0f,  1.0f,   -0.0f,   0.975528f, 0.654508f,
                  0.0f, 0.5f,       0.0f,      -0.0f,  1.0f,   -0.0f,        0.5f,      0.5f,
             0.475528f, 0.5f, -0.154509f,      -0.0f,  1.0f,   -0.0f,   0.975528f, 0.654508f,
                  0.0f, 0.5f,      -0.5f,      -0.0f,  1.0f,   -0.0f,        0.5f,      1.0f,
                  0.0f, 0.0f,      -0.5f,   -0.5878f, -0.0f, -0.809f,        0.5f,      1.0f,
            -0.475528f, 0.0f, -0.154509f,   -0.5878f, -0.0f, -0.809f,   0.024472f, 0.654508f,
            -0.475528f, 0.5f, -0.154509f,   -0.5878f, -0.0f, -0.809f,   0.024472f, 0.654508f,
                  0.0f, 0.0f,      -0.5f,   -0.5878f, -0.0f, -0.809f,        0.5f,      1.0f,
            -0.475528f, 0.5f, -0.154509f,   -0.5878f, -0.0f, -0.809f,   0.024472f, 0.654508f,
                  0.0f, 0.5f,      -0.5f,   -0.5878f, -0.0f, -0.809f,        0.5f,      1.0f,
             0.475528f, 0.0f, -0.154509f,    0.5878f, -0.0f, -0.809f,   0.975528f, 0.654508f,
                  0.0f, 0.0f,      -0.5f,    0.5878f, -0.0f, -0.809f,        0.5f,      1.0f,
                  0.0f, 0.5f,      -0.5f,    0.5878f, -0.0f, -0.809f,        0.5f,      1.0f,
             0.475528f, 0.0f, -0.154509f,    0.5878f, -0.0f, -0.809f,   0.975528f, 0.654508f,
                  0.0f, 0.5f,      -0.5f,    0.5878f, -0.0f, -0.809f,        0.5f,      1.0f,
             0.475528f, 0.5f, -0.154509f,    0.5878f, -0.0f, -0.809f,   0.975528f, 0.654508f,
             0.293893f, 0.0f,  0.404509f,    0.9511f, -0.0f,  0.309f,   0.793893f, 0.095491f,
             0.475528f, 0.0f, -0.154509f,    0.9511f, -0.0f,  0.309f,   0.975528f, 0.654508f,
             0.475528f, 0.5f, -0.154509f,    0.9511f, -0.0f,  0.309f,   0.975528f, 0.654508f,
             0.293893f, 0.0f,  0.404509f,    0.9511f, -0.0f,  0.309f,   0.793893f, 0.095491f,
             0.475528f, 0.5f, -0.154509f,    0.9511f, -0.0f,  0.309f,   0.975528f, 0.654508f,
             0.293893f, 0.5f,  0.404509f,    0.9511f, -0.0f,  0.309f,   0.793893f, 0.095491f,
            -0.293893f, 0.0f,  0.404509f,      -0.0f, -0.0f,    1.0f,   0.206107f, 0.095491f,
             0.293893f, 0.0f,  0.404509f,      -0.0f, -0.0f,    1.0f,   0.793893f, 0.095491f,
             0.293893f, 0.5f,  0.404509f,      -0.0f, -0.0f,    1.0f,   0.793893f, 0.095491f,
            -0.293893f, 0.0f,  0.404509f,      -0.0f, -0.0f,    1.0f,   0.206107f, 0.095491f,
             0.293893f, 0.5f,  0.404509f,      -0.0f, -0.0f,    1.0f,   0.793893f, 0.095491f,
            -0.293893f, 0.5f,  0.404509f,      -0.0f, -0.0f,    1.0f,   0.206107f, 0.095491f,
            -0.475528f, 0.0f, -0.154509f,   -0.9511f, -0.0f,  0.309f,   0.024472f, 0.654508f,
            -0.293893f, 0.0f,  0.404509f,   -0.9511f, -0.0f,  0.309f,   0.206107f, 0.095491f,
            -0.293893f, 0.5f,  0.404509f,   -0.9511f, -0.0f,  0.309f,   0.206107f, 0.095491f,
            -0.475528f, 0.0f, -0.154509f,   -0.9511f, -0.0f,  0.309f,   0.024472f, 0.654508f,
            -0.293893f, 0.5f,  0.404509f,   -0.9511f, -0.0f,  0.309f,   0.206107f, 0.095491f,
            -0.475528f, 0.5f, -0.154509f,   -0.9511f, -0.0f,  0.309f,   0.024472f, 0.654508f
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
                std::cerr << "Cube::GenerateMesh(): Error al cargar la textura " << m_DiffuseTexturePath << "\n";
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
                std::cerr << "Cube::GenerateMesh(): Error al cargar la textura " << m_SpecularTexturePath << "\n";
        }

		return std::make_shared<Mesh>(vertices, textures);
	}
}