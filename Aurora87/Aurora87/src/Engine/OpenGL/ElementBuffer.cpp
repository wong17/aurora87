#include "ElementBuffer.h"

namespace Engine
{
	ElementBuffer::ElementBuffer(const uint32_t* indices, uint32_t count, GLenum usage) 
		: m_Count(count), m_RendererID(0), m_Usage(usage), m_Size(count * sizeof(uint32_t))
	{
		// Since OpenGL 4.5 DSA 
		glCreateBuffers(1, &m_RendererID);
		GLCall(glNamedBufferData(m_RendererID, m_Size, indices, usage));
	}

	ElementBuffer::ElementBuffer(const std::vector<uint32_t>& indices, GLenum usage)
		: m_Count(static_cast<uint32_t>(indices.size())), m_RendererID(0), m_Usage(usage)
	{
		m_Size = m_Count * sizeof(uint32_t);
		glCreateBuffers(1, &m_RendererID);
		GLCall(glNamedBufferData(m_RendererID, m_Size, indices.data(), usage));
	}

	ElementBuffer::~ElementBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void ElementBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void ElementBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}