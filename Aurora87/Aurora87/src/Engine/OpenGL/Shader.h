#pragma once

#include "Engine/Util/Utils.h"
#include "Engine/Core/DebugGL.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <memory>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <cstring>
#include <algorithm>

namespace Engine
{
	class Shader
	{
	public:
		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();
		
		void Bind() const;
		void Unbind() const;
		
		void SetInt(const std::string& name, int value);
		void SetIntArray(const std::string& name, int* values, uint32_t count);
		void SetFloat(const std::string& name, float value);
		void SetFloat2(const std::string& name, const glm::vec2& value);
		void SetFloat3(const std::string& name, const glm::vec3& value);
		void SetFloat4(const std::string& name, const glm::vec4& value);
		void SetMat2(const std::string& name, const glm::mat2& value);
		void SetMat3(const std::string& name, const glm::mat3& value);
		void SetMat4(const std::string& name, const glm::mat4& value);
		bool HasUniformBlock(const std::string& blockName) const;

		inline uint32_t GetRendererID() const { return m_RendererID; }
		inline const std::string& GetName() const { return m_Name; }
		inline const std::string& GetVertexFilePath() const { return m_VertexFilePath; }
		inline const std::string& GetFragmentFilePath() const { return m_FragmentFilePath; }
		int GetUniformBlockIndex(const std::string& blockName) const;

		static std::shared_ptr<Shader> Create(const std::string& vertexPath, const std::string& fragmentPath);
		static std::shared_ptr<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

	private:
		std::string ReadFile(const std::string& filePath);
		int GetUniformLocation(const std::string& name) const;
		uint32_t CompileShader(uint32_t type, const std::string& source);
		void CreateShaderProgram(const std::string& vertexSrc, const std::string& fragmentSrc);
	private:
		uint32_t m_RendererID;
		std::string m_Name;
		std::string m_VertexFilePath;
		std::string m_FragmentFilePath;
		mutable std::unordered_map<std::string, int> m_UniformLocationCache;
		mutable std::unordered_map<std::string, int> m_UniformBlockIndexCache;
	};
}