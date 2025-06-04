#pragma once

#include <cstdint>

namespace Engine
{
	// Estos son los diferentes tipos de datos que se pueden usar en los shaders.
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static unsigned int ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:		return 4;
			case ShaderDataType::Float2:	return 4 * 2;
			case ShaderDataType::Float3:	return 4 * 3;
			case ShaderDataType::Float4:	return 4 * 4;
			case ShaderDataType::Mat3:		return 4 * 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4 * 4;
			case ShaderDataType::Int:		return 4;
			case ShaderDataType::Int2:		return 4 * 2;
			case ShaderDataType::Int3:		return 4 * 3;
			case ShaderDataType::Int4:		return 4 * 4;
			case ShaderDataType::Bool:		return 1;
		}
		return 0;
	}

    static uint32_t Std140BaseAlignment(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float:   return 4;
            case ShaderDataType::Float2:  return 8;
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:  return 16;
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4:    return 16;
            case ShaderDataType::Int:     return 4;
            case ShaderDataType::Int2:    return 8;
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:    return 16;
            case ShaderDataType::Bool:    return 4;
            default:                      return 4;
        }
    }

    static uint32_t Std140Size(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float:   return 4;
            case ShaderDataType::Float2:  return 8;
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:  return 16;
            case ShaderDataType::Mat3:    return 16 * 3;
            case ShaderDataType::Mat4:    return 16 * 4;
            case ShaderDataType::Int:     return 4;
            case ShaderDataType::Int2:    return 8;
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:    return 16;
            case ShaderDataType::Bool:    return 4;
            default:                      return 0;
        }
    }
}