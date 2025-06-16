#include "VertexBuffer.h"

namespace Engine
{
	VertexBuffer::VertexBuffer(uint32_t size, GLenum usage)
		: m_Size(size), m_Capacity(size), m_Usage(usage)
	{
		// Since OpenGL 4.5 DSA (Direct State Access)
		glCreateBuffers(1, &m_RendererID);
		GLCall(glNamedBufferData(m_RendererID, size, nullptr, usage));
	}

	VertexBuffer::VertexBuffer(float* vertices, uint32_t size, GLenum usage)
		: m_Size(size), m_Capacity(size), m_Usage(usage)
	{
		// Since OpenGL 4.5 DSA (Direct State Access)
		glCreateBuffers(1, &m_RendererID);
		GLCall(glNamedBufferData(m_RendererID, size, vertices, usage));
	}

	VertexBuffer::VertexBuffer(const std::vector<glm::mat4>& matrices, GLenum usage)
		: m_Usage(usage)
	{
		std::vector<float> raw;
		raw.reserve(matrices.size() * 16);
		for (const auto& mat : matrices) 
		{
			for (int j = 0; j < 4; ++j) 
			{
				raw.emplace_back(mat[j][0]);
				raw.emplace_back(mat[j][1]);
				raw.emplace_back(mat[j][2]);
				raw.emplace_back(mat[j][3]);
			}
		}
		m_Size = static_cast<uint32_t>(raw.size() * sizeof(float));
		m_Capacity = m_Size;

		glCreateBuffers(1, &m_RendererID);
		GLCall(glNamedBufferData(m_RendererID, m_Size, raw.data(), usage));
	}

	VertexBuffer::VertexBuffer(const std::vector<float>& vertices, GLenum usage)
		: m_Usage(usage)
	{
		m_Size = static_cast<uint32_t>(vertices.size() * sizeof(float));
		m_Capacity = m_Size;
		glCreateBuffers(1, &m_RendererID);
		GLCall(glNamedBufferData(m_RendererID, m_Size, vertices.data(), usage));
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::SetData(const void* data, uint32_t size)
	{
		if (size > m_Capacity) 
		{
			// Resizes the entire buffer
			m_Capacity = size;
			m_Size = size;
			GLCall(glNamedBufferData(m_RendererID, m_Capacity, data, m_Usage));
		}
		else 
		{
			// Only sub-data with DSA
			m_Size = size;
			GLCall(glNamedBufferSubData(m_RendererID, 0, m_Size, data));
		}
	}
}