#include "ShaderManager.h"

namespace Engine
{
	void ShaderManager::Add(const std::string& name, const std::shared_ptr<Shader>& shader)
	{
		if (!shader)
		{
			std::cerr << "ShaderManager::Add Shader es null.\n";
			return;
		}
		if (name.empty())
		{
			std::cerr << "ShaderManager::Add Shader name esta vacio.\n";
			return;
		}

		if (Exists(name)) {
			std::cout << "ShaderManager: Shader '" << name << "' ya existe.\n";
			return;
		}

		m_Shaders[name] = shader;
	}

	void ShaderManager::Add(const std::shared_ptr<Shader>& shader)
	{
		if (!shader)
		{
			std::cerr << "ShaderManager::Add Shader es null.\n";
			return;
		}

		const std::string name = shader->GetName();
		Add(name, shader);
	}

	std::shared_ptr<Shader> ShaderManager::Load(const std::string& vertexPath, const std::string& fragmentPath)
	{
		auto shader = Shader::Create(vertexPath, fragmentPath);
		const std::string& name = shader->GetName();

		if (Exists(name)) {
			std::cout << "ShaderManager: Shader '" << name << "' ya está cargado. Se reutilizará.\n";
			return Get(name);
		}

		Add(name, shader);
		return shader;
	}

	std::shared_ptr<Shader> ShaderManager::Load(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
	{
		if (Exists(name)) {
			std::cout << "ShaderManager: Shader '" << name << "' ya está cargado. Se reutilizará.\n";
			return Get(name);
		}

		auto shader = Shader::Create(name, vertexPath, fragmentPath);
		Add(name, shader);
		return shader;
	}

	std::shared_ptr<Shader> ShaderManager::Get(const std::string& name)
	{
		auto it = m_Shaders.find(name);
		if (it == m_Shaders.end()) {
			std::cerr << "ShaderManager: Shader '" << name << "' no existe.\n";
			return nullptr;
		}

		return it->second;
	}

	bool ShaderManager::TryGet(const std::string& name, std::shared_ptr<Shader>& outShader) const
	{
		auto it = m_Shaders.find(name);
		if (it == m_Shaders.end()) return false;

		outShader = it->second;

		return true;
	}

	bool ShaderManager::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}

	bool ShaderManager::Exists(const std::shared_ptr<Shader>& shader) const
	{
		if (!shader) return false;

		for (auto& [key, sh] : m_Shaders) {
			if (sh == shader) return true;
		}

		return false;
	}

	void ShaderManager::Remove(const std::string& name)
	{
		auto it = m_Shaders.find(name);
		if (it == m_Shaders.end()) {
			std::cerr << "ShaderManager: Shader '" << name << "' no existe.\n";
			return;
		}
		m_Shaders.erase(it);
	}

	void ShaderManager::Remove(const std::shared_ptr<Shader>& shader)
	{
		if (!shader) return;

		for (auto it = m_Shaders.begin(); it != m_Shaders.end(); ++it) {
			if (it->second == shader) {
				m_Shaders.erase(it);
				return;
			}
		}
		std::cerr << "ShaderManager: Shader no encontrado.\n";
	}

	void ShaderManager::ListShaders() const
	{
		std::cout << "Shaders cargados:\n";
		for (const auto& [name, shader] : m_Shaders) {
			std::cout << " - " << name << "\n";
		}
	}
}