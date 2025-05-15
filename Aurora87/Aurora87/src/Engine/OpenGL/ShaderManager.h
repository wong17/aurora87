#pragma once

#include "Shader.h"
#include "Engine/Util/Utils.h"

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

namespace Engine
{
	class ShaderManager
	{
	public:
		inline static ShaderManager& Get()
		{
			static ShaderManager s_Instance;
			return s_Instance;
		}

		void Add(const std::string& name, const std::shared_ptr<Shader>& shader);
		void Add(const std::shared_ptr<Shader>& shader);
		std::shared_ptr<Shader> Load(const std::string& vertexPath, const std::string& fragmentPath);
		std::shared_ptr<Shader> Load(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);

		std::shared_ptr<Shader> Get(const std::string& name);
		bool TryGet(const std::string& name, std::shared_ptr<Shader>& outShader) const;

		bool Exists(const std::string& name) const;
		bool Exists(const std::shared_ptr<Shader>& shader) const;

		void Remove(const std::string& name);
		void Remove(const std::shared_ptr<Shader>& shader);
		inline void Clear() { m_Shaders.clear(); }

		void ListShaders() const;

	private:
		ShaderManager() = default;
		~ShaderManager() = default;

		ShaderManager(const ShaderManager&) = delete;
		ShaderManager& operator=(const ShaderManager&) = delete;

		std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
	};
}