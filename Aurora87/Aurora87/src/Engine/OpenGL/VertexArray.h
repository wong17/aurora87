#pragma once

#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Engine/Core/DebugGL.h"

#include <glad/glad.h>

#include <cstdint>
#include <vector>
#include <memory>

namespace Engine
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void Bind() const;
		void Unbind() const;
		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer, uint32_t divisor = 0);
		void SetElementBuffer(const std::shared_ptr<ElementBuffer>& elementBuffer);

		inline uint32_t GetRendererID() const { return m_RendererID; }
		inline uint32_t GetVertexBufferIndex() const { return m_VertexBufferIndex; }
		const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
		const std::shared_ptr<ElementBuffer>& GetElementBuffer() const { return m_ElementBuffer; }

	private:
		uint32_t m_RendererID;
		uint32_t m_VertexBufferIndex = 0;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<ElementBuffer> m_ElementBuffer;
	};
}