#pragma once

#include "Engine/OpenGL/Mesh.h"
#include "Engine/Meshes/PrimitiveMesh.h"
#include "Engine/Meshes/Cube.h"
#include "Engine/Meshes/Pyramid.h"
#include "Engine/Meshes/Pentagon.h"
#include "Engine/Meshes/Hexagon.h"
#include "Engine/Meshes/Icosphere.h"
#include "Engine/Meshes/Plane.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <string>

namespace Engine
{
	class PrimitiveFactory
	{
	private:
		// Ahora textura va primero, tamaño al final
		template<typename PrimitiveType, typename TextureArg>
		static std::shared_ptr<Mesh> CreateInternal(TextureArg&& texture,
			const glm::vec3& size = glm::vec3(1.0f))
		{
			return std::make_shared<PrimitiveType>(
				std::forward<TextureArg>(texture),
				size
			)->GenerateMesh();
		}

#define REGISTER_PRIMITIVE(name, type)					\
        static std::shared_ptr<Mesh> Create##name(		\
            const std::string& path,					\
            const glm::vec3& size = glm::vec3(1.0f))	\
        {												\
            return CreateInternal<type>(path, size);	\
        }												\
        static std::shared_ptr<Mesh> Create##name(		\
            const std::shared_ptr<Texture>& texture,	\
            const glm::vec3& size = glm::vec3(1.0f))	\
        {												\
            return CreateInternal<type>(texture, size);	\
        }

	public:
		REGISTER_PRIMITIVE(Cube, Cube)
		REGISTER_PRIMITIVE(Pyramid, Pyramid)
		REGISTER_PRIMITIVE(Pentagon, Pentagon)
		REGISTER_PRIMITIVE(Hexagon, Hexagon)
		REGISTER_PRIMITIVE(Icosphere, Icosphere)
		REGISTER_PRIMITIVE(Plane, Plane)

#undef REGISTER_PRIMITIVE
	};
}