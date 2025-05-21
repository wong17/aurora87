#pragma once

#include "Engine/OpenGL/Mesh.h"
#include "Engine/OpenGL/Texture.h"
#include "Engine/OpenGL/AssimpTypes.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>

namespace Engine
{
	class PrimitiveMesh
	{
	public:
		PrimitiveMesh(const std::string& texturePath, glm::vec3 size = glm::vec3(1.0f))
			: m_Size(size), m_TexturePath(texturePath) { }

		PrimitiveMesh(std::shared_ptr<Texture> texture, glm::vec3 size = glm::vec3(1.0f))
			: m_Size(size), m_TexturePath(""), m_Texture(std::move(texture)) { }

		virtual ~PrimitiveMesh() = default;
		virtual std::shared_ptr<Mesh> GenerateMesh() const = 0;

	protected:
		glm::vec3 m_Size;
		std::string m_TexturePath;
		std::shared_ptr<Texture> m_Texture;
	};
}