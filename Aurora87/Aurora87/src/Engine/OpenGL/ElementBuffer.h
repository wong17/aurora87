#pragma once

#include "Engine/Core/DebugGL.h"

#include <glad/glad.h>
#include <cstdint>
#include <vector>

namespace Engine
{
	class ElementBuffer
	{
	public:
		ElementBuffer(const uint32_t* indices, uint32_t count, GLenum usage = GL_STATIC_DRAW);
		ElementBuffer(const std::vector<uint32_t>& indices, GLenum usage = GL_STATIC_DRAW);
		~ElementBuffer();

		void Bind() const;
		void Unbind() const;

		inline uint32_t GetRendererID() const { return m_RendererID; }
		inline GLenum GetUsage() const { return m_Usage; }
		inline uint32_t GetSize() const { return m_Size; }
		inline uint32_t GetCount() const { return m_Count; }

	private:
		uint32_t m_RendererID;
		GLenum m_Usage;
		uint32_t m_Size;	// m_Count * sizeof(uint32_t) en bytes
		uint32_t m_Count;	// number of items
	};
}