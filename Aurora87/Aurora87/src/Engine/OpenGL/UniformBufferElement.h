#pragma once

#include "ShaderDataTypes.h"

#include <cstdint>
#include <string>

namespace Engine
{
	struct UniformBufferElement
	{
		UniformBufferElement(const std::string& name, uint32_t size)
			: Name(name), Size(size), Offset(0), Type(ShaderDataType::None) { }
		UniformBufferElement(ShaderDataType type, const std::string& name)
			: Name(name), Size(ShaderDataTypeSize(type)), Offset(0), Type(type) { }

		std::string Name;
		uint32_t Size;
		uint32_t Offset;
		ShaderDataType Type;
	};
}