#pragma once

#include "Engine/OpenGL/Texture.h"
#include <assimp/scene.h>
#include <memory>
#include <cstdint>
#include <iostream>

namespace Engine
{
	class AssimpTextureLoader
	{
	public:
		// Carga una aiTexture embebida y retorna un Texture listo para usar.
		static std::shared_ptr<Texture> LoadEmbeddedTexture(aiTexture* texture);
		static std::shared_ptr<Texture> LoadEmbeddedTexture(aiTexture* texture, const TextureSpecification& spec);
	};
}