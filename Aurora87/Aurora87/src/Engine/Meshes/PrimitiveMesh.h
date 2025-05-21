#pragma once

#include "Engine/OpenGL/Mesh.h"
#include "Engine/OpenGL/Texture.h"
#include "Engine/OpenGL/AssimpTypes.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <string>

namespace Engine
{
	class PrimitiveMesh
	{
	public:
        PrimitiveMesh(const std::string& diffuseTexturePath, const std::string& speculaTexturePath = "")
            : m_DiffuseTexturePath(diffuseTexturePath), m_SpecularTexturePath(speculaTexturePath), 
			m_DiffuseTexture(nullptr), m_SpecularTexture(nullptr)
        { }

        PrimitiveMesh(std::shared_ptr<Texture> diffuseTexture, std::shared_ptr<Texture> specularTexture = nullptr)
            : m_DiffuseTexturePath(""), m_SpecularTexturePath(""), 
			m_DiffuseTexture(diffuseTexture), m_SpecularTexture(specularTexture)
        { }

		virtual ~PrimitiveMesh() = default;
		virtual std::shared_ptr<Mesh> GenerateMesh() const = 0;

	protected:
		std::string m_DiffuseTexturePath;
		std::string m_SpecularTexturePath;
		std::shared_ptr<Texture> m_DiffuseTexture = nullptr;
		std::shared_ptr<Texture> m_SpecularTexture = nullptr;
	};
}