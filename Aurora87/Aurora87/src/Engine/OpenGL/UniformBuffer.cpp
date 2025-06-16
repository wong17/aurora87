#include "UniformBuffer.h"

namespace Engine
{
	UniformBuffer::UniformBuffer(const UniformBufferLayout& layout, uint32_t bindingPoint, uint32_t blockCount)
		: m_Layout(layout), m_BindingPoint(bindingPoint), m_BlockCount(blockCount)
	{
		// Calculate “raw” stride (sum of element sizes)
		m_Stride = m_Layout.GetStride();

		// Obtain minimum required alignment per GPU
		GLint alignment = 0;
		glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &alignment);
		// Set ‘m_Stride’ to the next multiple of ‘alignment’ (align up).
		m_AlignedStride = ((m_Stride + alignment - 1) / alignment) * alignment;

		GLint maxUniformBlockSize = 0;
		glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBlockSize);
		// Validate that the individual stride is not larger than the allowed block.
		if (m_AlignedStride > static_cast<uint32_t>(maxUniformBlockSize)) 
		{
			throw std::runtime_error(
				"UniformBuffer::UniformBuffer: Layout exceeds maximum size allowed by GPU (" +
				std::to_string(m_AlignedStride) + " > " + std::to_string(maxUniformBlockSize) + " bytes)"
			);
		}

		// Validate that the total buffer size also does not exceed the maximum buffer size.
		GLsizeiptr totalSize = static_cast<GLsizeiptr>(m_AlignedStride) * m_BlockCount;
		if (totalSize > static_cast<GLsizeiptr>(maxUniformBlockSize)) 
		{
			throw std::runtime_error(
				"UniformBuffer::UniformBuffer: The total buffer size exceeds the maximum allowed. (" +
				std::to_string(totalSize) + " > " + std::to_string(maxUniformBlockSize) + " bytes)"
			);
		}
		
		// Create buffer and reserve immutable storage
		glCreateBuffers(1, &m_RendererID);
		GLCall(glNamedBufferStorage(m_RendererID, totalSize, nullptr, GL_DYNAMIC_STORAGE_BIT));
		// Binding the buffer to the tie point for use in the shader
		GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, m_BindingPoint, m_RendererID));
	}

	UniformBuffer::~UniformBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void UniformBuffer::Bind() const
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, m_BindingPoint, m_RendererID);
	}

	void UniformBuffer::Unbind() const
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, m_BindingPoint, 0);
	}

	/*
	 * This method binds to the shader only a subregion of the UBO, defined by ‘offset’ and ‘size’.
	 * offset': position in bytes where the block of an entity starts.
	 * ‘size’: byte size of the block (usually ‘alignedStride’)
	 */
	void UniformBuffer::BindRange(uint32_t offset, uint32_t size) const
	{
		glBindBufferRange(GL_UNIFORM_BUFFER, m_BindingPoint, m_RendererID, offset, size);
	}

	void UniformBuffer::SetData(const std::string& name, const void* data) const
	{
		const auto& elem = m_Layout.GetElement(name);
		GLCall(glNamedBufferSubData(m_RendererID, elem.Offset, elem.Size, data));
	}

	void UniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset) const
	{
		GLCall(glNamedBufferSubData(m_RendererID, offset, size, data));
	}

	void UniformBuffer::BindToShader(const Shader& shader, const std::string& blockName, uint32_t bindingPoint)
	{
		m_BindingPoint = bindingPoint;
		// Get the index of the uniform block in shader
		GLuint blockIndex = glGetUniformBlockIndex(shader.GetRendererID(), blockName.c_str());
		if (blockIndex == GL_INVALID_INDEX) 
		{
			std::cerr << "UniformBuffer::BindToShader: block '" << blockName << "' does not exist in shader '" << shader.GetName() << "'\n";
			return;
		}
		// Associate the uniform block to the tie point
		GLCall(glUniformBlockBinding(shader.GetRendererID(), blockIndex, bindingPoint));
		// Binding the Uniform Buffer to the tie point
		Bind();
	}

	void UniformBuffer::BindToShader(const std::string& blockName, uint32_t bindingPoint, 
		std::initializer_list<std::reference_wrapper<const Shader>> shaders)
	{
		m_BindingPoint = bindingPoint;

		for (const Shader& shader : shaders)
		{
			GLuint blockIndex = glGetUniformBlockIndex(shader.GetRendererID(), blockName.c_str());
			if (blockIndex == GL_INVALID_INDEX) 
			{
				std::cerr << "UniformBuffer::BindToShader: block '"	<< blockName << "' does not exist in shader '" << shader.GetName() << "'\n";
				continue;
			}
			GLCall(glUniformBlockBinding(shader.GetRendererID(), blockIndex, bindingPoint));
		}
		Bind();
	}

	void UniformBuffer::BindToShader(const std::string& blockName, std::initializer_list<std::reference_wrapper<const Shader>> shaders)
	{
		BindToShader(blockName, m_BindingPoint, shaders);
	}

	void UniformBuffer::ResizeBlocks(uint32_t blockCount)
	{
		if (blockCount <= m_BlockCount) return;
		
		// Save the old size for copying
		GLsizeiptr oldTotalSize = static_cast<GLsizeiptr>(m_AlignedStride) * m_BlockCount;
		GLsizeiptr newTotalSize = static_cast<GLsizeiptr>(m_AlignedStride) * blockCount;

		// Create new buffer
		GLuint newBuffer;
		glCreateBuffers(1, &newBuffer);

		// Allocate storage to the new buffer (allowing then glCopyNamedBufferSubData)
		GLCall(glNamedBufferStorage(newBuffer,	newTotalSize, nullptr, GL_DYNAMIC_STORAGE_BIT));

		// Copy old part to new buffer
		GLCall(glCopyNamedBufferSubData(
			m_RendererID,   // srcBuffer
			newBuffer,      // dstBuffer
			0,              // readOffset
			0,              // writeOffset
			oldTotalSize    // size to copy
		));

		// We release the old buffer
		glDeleteBuffers(1, &m_RendererID);

		// Save the new buffer ID
		m_RendererID = newBuffer;
		m_BlockCount = blockCount;

		// Link the new buffer to the tie point
		GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, m_BindingPoint, m_RendererID));
	}
}