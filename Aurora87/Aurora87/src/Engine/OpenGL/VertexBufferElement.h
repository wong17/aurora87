#pragma once

#include "ShaderDataTypes.h"

#include <glad/glad.h>

#include <string>

namespace Engine
{
	// Represents an element in the VertexBufferLayout and each element describes a VertexBufferAttribute
	class VertexBufferElement
	{
	public:
		VertexBufferElement() = default;
		VertexBufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: m_Type(type), m_Name(name), m_Size(ShaderDataTypeSize(type)), m_Offset(0), m_Normalized(normalized) {
		}

		void SetOffset(uint32_t offset) { m_Offset = offset; }

		uint32_t GetSize() const { return m_Size; }
		size_t GetOffset() const { return m_Offset; }
		const std::string& GetName() const { return m_Name; }
		ShaderDataType GetType() const { return m_Type; }
		bool GetNormalized() const { return m_Normalized; }

		// This is to know how many components the data type being used has.
		uint32_t GetComponentCount() const
		{
			switch (m_Type)
			{
				case ShaderDataType::Float:   return 1;
				case ShaderDataType::Float2:  return 2;
				case ShaderDataType::Float3:  return 3;
				case ShaderDataType::Float4:  return 4;
				case ShaderDataType::Mat3:    return 3; // Mat3 = 3 * vec3, each row of the matrix is a vec3
				case ShaderDataType::Mat4:    return 4; // Mat4 = 4 * vec4, each row of the matrix is a vec4
				case ShaderDataType::Int:     return 1;
				case ShaderDataType::Int2:    return 2;
				case ShaderDataType::Int3:    return 3;
				case ShaderDataType::Int4:    return 4;
				case ShaderDataType::Bool:    return 1;
			}
			return 0;
		}

	private:
		std::string m_Name;
		ShaderDataType m_Type;
		uint32_t m_Size;
		size_t m_Offset;
		bool m_Normalized;
	};
}