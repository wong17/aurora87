#pragma once

#include "VertexBufferElement.h"

#include <initializer_list>
#include <vector>
#include <cstdint>

namespace Engine
{
	class VertexBufferLayout
	{
	public:
		VertexBufferLayout() = default;
		VertexBufferLayout(std::initializer_list<VertexBufferElement> elements)
			: m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}
		~VertexBufferLayout() = default;

		std::vector<VertexBufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<VertexBufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<VertexBufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<VertexBufferElement>::const_iterator end() const { return m_Elements.end(); }

		inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
		inline uint32_t GetStride() const { return m_Stride; }
		inline uint32_t GetVertexCount(uint32_t bufferSizeInBytes) const
		{
			if (m_Stride == 0) 
				return 0;
			
			return bufferSizeInBytes / m_Stride;
		}

	private:
		void CalculateOffsetsAndStride()
		{
			uint32_t offset = 0; // It is the offset of each VertexBufferElement in the VertexBuffer
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.SetOffset(offset);
				offset += element.GetSize();
				m_Stride += element.GetSize();
			}
		}
	private:
		std::vector<VertexBufferElement> m_Elements;
		uint32_t m_Stride = 0;	// This is the total size of the VertexBuffer in the VertexArray
	};
}