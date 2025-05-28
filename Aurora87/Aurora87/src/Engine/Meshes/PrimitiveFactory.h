#pragma once

#include "Engine/OpenGL/Mesh.h"
#include "Engine/OpenGL/Texture.h"
#include "Engine/Meshes/PrimitiveMesh.h"
#include "Engine/Meshes/Cube.h"
#include "Engine/Meshes/Pyramid.h"
#include "Engine/Meshes/Pentagon.h"
#include "Engine/Meshes/Hexagon.h"
#include "Engine/Meshes/Icosphere.h"
#include "Engine/Meshes/Plane.h"
#include "Engine/Meshes/RectButton.h"

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
		// Versión que construye la primitiva a partir de rutas
		template<typename PrimitiveType>
		static std::shared_ptr<Mesh> CreateInternal(
			const std::string& diffusePath,
			const std::string& specularPath)
		{
			return std::make_shared<PrimitiveType>(
				diffusePath,
				specularPath
			)->GenerateMesh();
		}

		// Versión que construye la primitiva a partir de texturas ya cargadas
		template<typename PrimitiveType>
		static std::shared_ptr<Mesh> CreateInternal(
			const std::shared_ptr<Texture>& diffuse,
			const std::shared_ptr<Texture>& specular)
		{
			return std::make_shared<PrimitiveType>(
				diffuse,
				specular
			)->GenerateMesh();
		}

#define REGISTER_PRIMITIVE(name, type)							\
    static std::shared_ptr<Mesh> Create##name(					\
        const std::string& diffusePath,							\
        const std::string& specularPath = "")					\
    {															\
        return CreateInternal<type>(diffusePath, specularPath);	\
    }															\
    static std::shared_ptr<Mesh> Create##name(					\
        const std::shared_ptr<Texture>& diffuse,				\
        const std::shared_ptr<Texture>& specular = nullptr)		\
    {															\
        return CreateInternal<type>(diffuse, specular);			\
    }

	public:
		REGISTER_PRIMITIVE(Cube, Cube)
		REGISTER_PRIMITIVE(Pyramid, Pyramid)
		REGISTER_PRIMITIVE(Pentagon, Pentagon)
		REGISTER_PRIMITIVE(Hexagon, Hexagon)
		REGISTER_PRIMITIVE(Icosphere, Icosphere)
		REGISTER_PRIMITIVE(Plane, Plane)
		REGISTER_PRIMITIVE(RectButton, RectButton)
#undef REGISTER_PRIMITIVE
	};
}