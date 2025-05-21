#pragma once

#include "VertexBufferLayout.h"
#include "Engine/Core/DebugGL.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <cstdint>
#include <vector>

namespace Engine
{
	class VertexBuffer
	{
	public:
		VertexBuffer(uint32_t size, GLenum usage = GL_STATIC_DRAW);
		VertexBuffer(float* vertices, uint32_t size, GLenum usage = GL_STATIC_DRAW);
		VertexBuffer(const std::vector<glm::mat4>& matrices, GLenum usage = GL_STATIC_DRAW);
		VertexBuffer(const std::vector<float>& vertices, GLenum usage = GL_STATIC_DRAW);
		~VertexBuffer();
		
		void Bind() const;
		void Unbind() const;
		void SetLayout(const VertexBufferLayout& layout) { m_Layout = layout; }
		void SetData(const void* data, uint32_t size);

		inline uint32_t GetRendererID() const { return m_RendererID; }
		inline uint32_t GetSize() const { return m_Size; }         // bytes usados
		inline uint32_t GetCapacity() const { return m_Capacity; } // bytes asignados
		inline GLenum GetUsage() const { return m_Usage; }
		inline const VertexBufferLayout& GetLayout() const { return m_Layout; }
		inline uint32_t GetVertexCount() const
		{
			return m_Layout.GetVertexCount(m_Size);
		}

	private:
		uint32_t m_RendererID;
		uint32_t m_Size;      // bytes efectivamente usados
		uint32_t m_Capacity;  // bytes asignados en la GPU
		GLenum m_Usage;
		VertexBufferLayout m_Layout;
	};
}