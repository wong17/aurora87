#pragma once

#include "Texture.h"
#include "Engine/Util/Utils.h"

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

namespace Engine
{
	class TextureManager
	{
	public:
		inline static TextureManager& Get()
		{
			static TextureManager s_Instance;
			return s_Instance;
		}

		void Add(const std::string& name, const std::shared_ptr<Texture>& texture);
		void Add(const std::shared_ptr<Texture>& texture);
		std::shared_ptr<Texture> Load(const std::string& path, const TextureSpecification& specification = TextureSpecification());
		std::shared_ptr<Texture> Load(const std::string& name, const std::string& path, const TextureSpecification& specification = TextureSpecification());
		
		std::shared_ptr<Texture> Get(const std::string& key);
		
		bool Exists(const std::string& name) const;
		bool Exists(const std::shared_ptr<Texture>& texture) const;
		
		void Remove(const std::shared_ptr<Texture>& texture);
		inline void Clear() { m_Textures.clear(); }
		void GarbageCollect();

		void ListTextures() const;

	private:
		TextureManager() = default;
		~TextureManager() = default;

		TextureManager(const TextureManager&) = delete;
		TextureManager& operator=(const TextureManager&) = delete;

		std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;
	};
}