#pragma once

#include "Engine/Meshes/PrimitiveMesh.h"
#include "Engine/OpenGL/TextureManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <memory>
#include <vector>
#include <iostream>

namespace Engine
{
	class Hexagon : public PrimitiveMesh
	{
	public:
		using PrimitiveMesh::PrimitiveMesh;

		std::shared_ptr<Mesh> GenerateMesh() const override;
	};
}