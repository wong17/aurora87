#include "TextureManager.h"

namespace Engine
{
	void TextureManager::Add(const std::string& name, const std::shared_ptr<Texture>& texture)
	{
		if (!texture)
		{
			std::cerr << "TextureManager::Add: Texture is null." << std::endl;
			return;
		}

		if (name.empty()) 
		{
			std::cerr << "TextureManager::Add: Texture name cannot be empty." << std::endl;
			return;
		}

		std::string key = name + "@" + texture->GetPath();
		if (Exists(key)) 
		{
			std::cout << "TextureManager::Add: Texture '" << name << "' already exists." << std::endl;
			return;
		}

		m_Textures[key] = texture;
	}

	void TextureManager::Add(const std::shared_ptr<Texture>& texture)
	{
		if (!texture)
		{
			std::cerr << "TextureManager::Add: Texture is null." << std::endl;
			return;
		}

		const std::string& name = texture->GetName();
		if (name.empty()) 
		{
			std::cerr << "TextureManager::Add: Texture name cannot be empty." << std::endl;
			return;
		}

		std::string key = name + "@" + texture->GetPath();
		if (Exists(name)) 
		{
			std::cerr << "TextureManager::Add: Texture '" << name << "' already exists." << std::endl;
			return;
		}

		m_Textures[key] = texture;
	}

	std::shared_ptr<Texture> TextureManager::Load(const std::string& path, const TextureSpecification& specification)
	{
		std::string name = Utils::ExtractFileName(path);
		return Load(name, path, specification);
	}

	std::shared_ptr<Texture> TextureManager::Load(const std::string& name, const std::string& path, const TextureSpecification& specification)
	{
		std::string key = name + "@" + path;
		if (Exists(key)) 
		{
			//std::cout << "TextureManager: Texture '" << name << "' already exists." << std::endl;
			return m_Textures[key];
		}
		
		auto texture = std::make_shared<Texture>(specification, path);
		if (!texture->IsLoaded()) 
		{
			std::cerr << "TextureManager::Load: Error loading texture '" << name << "'." << std::endl;
			return nullptr;
		}

		m_Textures[key] = texture;

		return texture;
	}

	std::shared_ptr<Texture> TextureManager::Get(const std::string& key)
	{
		if (!Exists(key)) 
		{
			std::cerr << "TextureManager: Texture '" << key << "' not found." << std::endl;
			return nullptr;
		}

		return m_Textures[key];
	}

	bool TextureManager::Exists(const std::string& name) const
	{
		return m_Textures.find(name) != m_Textures.end();
	}

	bool TextureManager::Exists(const std::shared_ptr<Texture>& texture) const
	{
		if (!texture) return false;

		for (const auto& [key, tex] : m_Textures) 
		{
			if (tex == texture)
				return true;
		}

		return false;
	}

	void TextureManager::Remove(const std::shared_ptr<Texture>& texture)
	{
		if (!texture) return;

		for (auto it = m_Textures.begin(); it != m_Textures.end(); ++it) 
		{
			if (it->second == texture) 
			{
				m_Textures.erase(it);
				return;
			}
		}

		std::cerr << "TextureManager: Texture no encontrada." << std::endl;
	}

	void TextureManager::GarbageCollect()
	{
		for (auto it = m_Textures.begin(); it != m_Textures.end(); ) 
		{
			if (it->second.use_count() == 1) 
			{  
				// If only the manager keeps it, we delete it.
				it = m_Textures.erase(it);
			}
			else 
			{
				++it;
			}
		}
	}

	void TextureManager::ListTextures() const
	{
		std::cout << "Loaded textures:\n";
		for (const auto& [name, texture] : m_Textures) 
		{
			std::cout << " - " << name << "\n";
		}
	}
}