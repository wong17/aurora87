#include "AssimpTextureLoader.h"

namespace Engine
{
	std::shared_ptr<Texture> AssimpTextureLoader::LoadEmbeddedTexture(aiTexture* texture)
	{
		TextureSpecification spec;
		return LoadEmbeddedTexture(texture, spec);
	}

	std::shared_ptr<Texture> AssimpTextureLoader::LoadEmbeddedTexture(aiTexture* texture, const TextureSpecification& spec)
	{
		int width, height, channels;
		unsigned char* data = nullptr;
		bool mustFree = false;

		if (texture->mHeight == 0)
		{
			// Textura comprimida en memoria (formato .jpg/.png/etc)
			data = stbi_load_from_memory(
				reinterpret_cast<stbi_uc*>(texture->pcData),
				static_cast<int>(texture->mWidth), 
				&width, &height, &channels, STBI_default);
			mustFree = true;
		}
		else
		{
			// raw RGBA data
			width = texture->mWidth;
			height = texture->mHeight;
			channels = 4;
			data = reinterpret_cast<unsigned char*>(texture->pcData);
		}

		if (!data) {
			std::cerr << "AssimpTextureLoader::LoadEmbeddedTexture: fallo al decodificar textura embebida\n";
			return nullptr;
		}

		// determinar formato
		ImageFormat fmt = ImageFormat::None;
		switch (channels)
		{
			case 1: fmt = ImageFormat::R8;		break;
			case 2: fmt = ImageFormat::RG8;		break;
			case 3: fmt = ImageFormat::RGB8;	break;
			case 4: fmt = ImageFormat::RGBA8;	break;
			default:
			{
				std::cerr << "AssimpTextureLoader::LoadEmbeddedTexture: formato de textura no soportado: " << channels << "\n";
				if (mustFree)
					stbi_image_free(data);

				return nullptr;
			}
		}

		TextureSpecification s = spec;
		s.Width = static_cast<uint32_t>(width);
		s.Height = static_cast<uint32_t>(height);
		s.Format = fmt;
		s.GenerateMips = spec.GenerateMips;

		auto tex = std::make_shared<Texture>(s);
		tex->SetData(data, width * height * channels);

		if (mustFree)
			stbi_image_free(data);

		return tex;
	}
}