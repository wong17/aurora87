#include "Shader.h"

namespace Engine
{
	static std::string GenerateShaderName(const std::string& vertexPath, const std::string& fragmentPath)
	{
		return Utils::ExtractFileName(vertexPath) + "_" + Utils::ExtractFileName(fragmentPath);
	}

	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) 
		: m_RendererID(0), m_VertexFilePath(vertexPath), m_FragmentFilePath(fragmentPath)
	{
		std::string vertexSource = ReadFile(vertexPath);
		std::string fragmentSource = ReadFile(fragmentPath);
		m_Name = GenerateShaderName(vertexPath, fragmentPath);
		CreateShaderProgram(vertexSource, fragmentSource);
	}

	Shader::Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) 
		: m_RendererID(0), m_Name(name)
	{
		CreateShaderProgram(vertexSrc, fragmentSrc);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::SetInt(const std::string& name, int value)
	{
		GLCall(glUniform1i(GetUniformLocation(name), value));
	}

	void Shader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		GLCall(glUniform1iv(GetUniformLocation(name), count, values));
	}

	void Shader::SetFloat(const std::string& name, float value)
	{
		GLCall(glUniform1f(GetUniformLocation(name), value));
	}

	void Shader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		GLCall(glUniform2f(GetUniformLocation(name), value.x, value.y));
	}

	void Shader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		GLCall(glUniform3f(GetUniformLocation(name), value.x, value.y, value.z));
	}

	void Shader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		GLCall(glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w));
	}

	void Shader::SetMat2(const std::string& name, const glm::mat2& value)
	{
		GLCall(glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value)));
	}

	void Shader::SetMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLCall(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix)));
	}

	void Shader::SetMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix)));
	}

	bool Shader::HasUniformBlock(const std::string& blockName) const
	{
		int index = GetUniformBlockIndex(blockName);
		if (index == -1)
		{
			std::cout << "Shader::HasUniformBlock: The block '" << blockName << "' does not exist.\n";
			return false;
		}
		return true;
	}

	int Shader::GetUniformBlockIndex(const std::string& blockName) const
	{
		auto it = m_UniformBlockIndexCache.find(blockName);
		if (it != m_UniformBlockIndexCache.end())
			return it->second;

		int index = glGetUniformBlockIndex(m_RendererID, blockName.c_str());
		m_UniformBlockIndexCache[blockName] = index;

		return index;
	}

	std::shared_ptr<Shader> Shader::Create(const std::string& vertexPath, const std::string& fragmentPath)
	{
		return std::make_shared<Shader>(vertexPath, fragmentPath);
	}

	std::shared_ptr<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		return std::make_shared<Shader>(name, vertexSrc, fragmentSrc);
	}

	std::string Shader::ReadFile(const std::string& filePath)
	{
		std::ifstream stream(filePath);
		if (!stream.is_open())
		{
			char errorMsg[256];
			strerror_s(errorMsg, sizeof(errorMsg), errno);

			std::cerr << "Shader::ReadFile: File could not be opened: " << filePath << std::endl;
			std::cerr << errorMsg << std::endl;
			return "";
		}

		std::stringstream ss;
		ss << stream.rdbuf();

		return ss.str();
	}

	int Shader::GetUniformLocation(const std::string& name) const
	{
		auto it = m_UniformLocationCache.find(name);
		if (it != m_UniformLocationCache.end())
			return it->second;

		int location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1)
		{
			//std::cout << "Shader::GetUniformLocation: uniform '" << name << "' does not exist or is not being used." << std::endl;
		}

		m_UniformLocationCache[name] = location;

		return location;
	}

	uint32_t Shader::CompileShader(uint32_t type, const std::string& source)
	{
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();

		GLCall(glShaderSource(id, 1, &src, nullptr));
		GLCall(glCompileShader(id));

		// Revisar errores de compilación
		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			char infoLog[512];
			glGetShaderInfoLog(id, 512, NULL, infoLog);
			std::cout << "Shader::CompileShader: " << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << " failure to compile\n" << std::endl;
			std::cout << infoLog << std::endl;

			glDeleteShader(id);
			return 0;
		}

		return id;
	}

	void Shader::CreateShaderProgram(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		uint32_t program = glCreateProgram();
		uint32_t vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSrc);
		uint32_t fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);

		GLCall(glAttachShader(program, vertexShader));
		GLCall(glAttachShader(program, fragmentShader));
		GLCall(glLinkProgram(program));

		// Revisar errores al linkear
		int success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			std::cout << "Shader::CreateShaderProgram : Failure to link\n" << std::endl;
			std::cout << infoLog << std::endl;

			glDeleteProgram(program);
			return;
		}

		GLCall(glValidateProgram(program));
		// Remove shaders as they are no longer needed after they have been linked.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		m_RendererID = program;
	}
}