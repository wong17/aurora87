#pragma once

#include "Engine/OpenGL/Texture.h"

#include <dds-ktx/dds-ktx.h>
#include <assimp/scene.h>
#include <stb_image/stb_image.h>

#include <memory>
#include <cstdint>
#include <iostream>
#include <algorithm>

namespace Engine
{
	class AssimpTextureLoader
	{
	public:
		// Loads an embedded aiTexture and returns a ready to use Texture.
		static std::shared_ptr<Texture> LoadEmbeddedTexture(aiTexture* aiTex);
		static std::shared_ptr<Texture> LoadEmbeddedTexture(aiTexture* aiTex, const TextureSpecification& spec);

	private:
		static std::shared_ptr<Texture> LoadStandard(aiTexture* aiTex, const TextureSpecification& spec);
		static std::shared_ptr<Texture> LoadDDS(aiTexture* aiTex, const TextureSpecification& spec);
		static std::shared_ptr<Texture> LoadKTX(aiTexture* aiTex, const TextureSpecification& spec);

		static ImageFormat ChannelsToFormat(int channels);
	};
}