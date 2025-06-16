#include "AssimpTextureLoader.h"

namespace Engine
{
	ImageFormat AssimpTextureLoader::ChannelsToFormat(int channels)
	{
		switch (channels) 
		{
			case 1: return ImageFormat::R8;
			case 2: return ImageFormat::RG8;
			case 3: return ImageFormat::RGB8;
			case 4: return ImageFormat::RGBA8;
			default:
			{
				std::cerr << "AssimpTextureLoader::ChannelsToFormat: texture format not supported: " << channels << "\n";
				return ImageFormat::None;
			}
		}
	}

	std::shared_ptr<Texture> AssimpTextureLoader::LoadEmbeddedTexture(aiTexture* aiTex)
	{
		TextureSpecification spec;
		return LoadEmbeddedTexture(aiTex, spec);
	}

	std::shared_ptr<Texture> AssimpTextureLoader::LoadEmbeddedTexture(aiTexture* aiTex, const TextureSpecification& spec)
	{
		bool isDDS = false;
		if (aiTex->mWidth >= 4) 
		{
			const uint8_t* data = reinterpret_cast<const uint8_t*>(aiTex->pcData);
			isDDS = (memcmp(data, "DDS ", 4) == 0);
		}

		std::string hint(aiTex->achFormatHint, 4);
		hint.erase(std::find(hint.begin(), hint.end(), '\0'), hint.end()); // Remover nulos
		std::transform(hint.begin(), hint.end(), hint.begin(), ::tolower);

		if(isDDS || hint.find("dds") != std::string::npos)
		{
			return LoadDDS(aiTex, spec);
		}
		else if (hint.find("ktx") != std::string::npos)
		{
			return LoadKTX(aiTex, spec);
		}
		
		return LoadStandard(aiTex, spec); // PNG, JPEG, etc.
	}

	std::shared_ptr<Texture> AssimpTextureLoader::LoadStandard(aiTexture* aiTex, const TextureSpecification& spec)
	{
		int width, height, channels;
		stbi_uc* data = nullptr;

		// Compressed texture in memory (format .jpg/.png/etc)
		data = stbi_load_from_memory(
			reinterpret_cast<stbi_uc*>(aiTex->pcData), static_cast<int>(aiTex->mWidth), &width, &height, &channels, STBI_default);

		if (!data) 
		{
			std::cerr << "AssimpTextureLoader::LoadEmbeddedTexture: failure to decode embedded texture\n";
			return nullptr;
		}

		TextureSpecification texSpec = spec;
		texSpec.Width = width;
		texSpec.Height = height;
		texSpec.Format = ChannelsToFormat(channels);
		texSpec.GenerateMips = spec.GenerateMips;

		auto texture = std::make_shared<Texture>(texSpec);
		texture->SetData(data, width * height * channels);
		stbi_image_free(data);
			
		return texture;
	}

	std::shared_ptr<Texture> AssimpTextureLoader::LoadDDS(aiTexture* aiTex, const TextureSpecification& spec)
	{
		ddsktx_texture_info info;
		const uint8_t* dds_data = reinterpret_cast<const uint8_t*>(aiTex->pcData);
		int dds_size = static_cast<int>(aiTex->mWidth);

		// Parse DDS block in memory
		if (!ddsktx_parse(&info, dds_data, dds_size, nullptr))
		{
			std::cerr << "AssimpTextureLoader::LoadDDS: failure when parsing embedded DDS\n";
			return nullptr;
		}

		// Determine if sRGB based on the format
		bool isSRGB = false;
		switch (info.format) 
		{
			case DDSKTX_FORMAT_BC1:
			case DDSKTX_FORMAT_BC3:
			case DDSKTX_FORMAT_BC7:
				isSRGB = (info.flags & DDSKTX_TEXTURE_FLAG_SRGB) != 0;
				break;
			default: break;
		}

		// Map ddsktx_format to our ImageFormat
		ImageFormat format = ImageFormat::None;
		switch (info.format) 
		{
			case DDSKTX_FORMAT_BC1:
				format = isSRGB ? ImageFormat::BC1_SRGB : ImageFormat::BC1_RGB;
				break;
			case DDSKTX_FORMAT_BC3:
				format = isSRGB ? ImageFormat::BC3_SRGBA : ImageFormat::BC3_RGBA;
				break;
			case DDSKTX_FORMAT_BC7:
				format = ImageFormat::BC7;
				break;
			default:
				std::cerr << "AssimpTextureLoader::LoadDDS: DDS format not supported: 0x" << std::hex << info.format << "\n";
				return nullptr;
		}

		// Preparing the OpenGL specification
		TextureSpecification texSpec = spec;
		texSpec.Width = info.width;
		texSpec.Height = info.height;
		texSpec.Format = format;
		texSpec.GenerateMips = false; // DDS already includes the mipmaps
		texSpec.IsCompressed = true;
		texSpec.MipLevels = info.num_mips;

		// Create texture and reserve space for N levels
		auto texture = std::make_shared<Texture>(texSpec);

		// Upload each compressed mipmap
		ddsktx_sub_data sub_data;
		for (int mip = 0; mip < info.num_mips; ++mip)
		{
			ddsktx_get_sub(&info, &sub_data,
				dds_data,					// pointer to DDS
				static_cast<int>(dds_size), // DDS size in bytes (cast to int)
				0,							// array layer
				0,							// cubemap face
				mip);						// mip level

			if (sub_data.buff && sub_data.size_bytes > 0)
			{
				texture->SetCompressedData(sub_data.buff, sub_data.size_bytes, mip);
			}
			else
			{
				std::cerr << "AssimpTextureLoader::LoadDDS: Error loading mipmap " << mip << "\n";
				break;
			}
		}

		return texture;
	}

	std::shared_ptr<Texture> AssimpTextureLoader::LoadKTX(aiTexture* aiTex, const TextureSpecification& spec)
	{
		return AssimpTextureLoader::LoadDDS(aiTex, spec);
	}
}