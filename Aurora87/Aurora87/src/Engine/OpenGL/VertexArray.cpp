#include "VertexArray.h"

namespace Engine
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:    return GL_FLOAT;
			case ShaderDataType::Float2:   return GL_FLOAT;
			case ShaderDataType::Float3:   return GL_FLOAT;
			case ShaderDataType::Float4:   return GL_FLOAT;
			case ShaderDataType::Mat3:     return GL_FLOAT;
			case ShaderDataType::Mat4:     return GL_FLOAT;
			case ShaderDataType::Int:      return GL_INT;
			case ShaderDataType::Int2:     return GL_INT;
			case ShaderDataType::Int3:     return GL_INT;
			case ShaderDataType::Int4:     return GL_INT;
			case ShaderDataType::Bool:     return GL_BOOL;
		}

		return 0;
	}

	VertexArray::VertexArray()
	{
		// Since OpenGL 4.5 DSA
		glCreateVertexArrays(1, &m_RendererID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer, uint32_t divisor)
	{
		const auto& layout = vertexBuffer->GetLayout();
		// BindingIndex is the buffer slot in the VAO.
		const uint32_t bindingIndex = static_cast<uint32_t>(m_VertexBuffers.size());
		m_VertexBuffers.push_back(vertexBuffer);
		// We add the VBO in the VAO using DSA (binding index)
		GLCall(glVertexArrayVertexBuffer(m_RendererID, bindingIndex, vertexBuffer->GetRendererID(), 0, layout.GetStride()));
		
		/*
		 * This function tells OpenGL how often to advance to the next value of an attribute when rendering using instancing.
		 * when rendering using instancing.
		 *
		 * Tells it when to update the contents of an attribute to the next element.
		 * If divisor = 0 (default), that attribute is not updated and the same value is used for all elements in the vertex buffer,
		 * If divisor = 1, the attribute content is updated for each instance drawn.
		 */
		if (divisor > 0)
			GLCall(glVertexArrayBindingDivisor(m_RendererID, bindingIndex, divisor));

		for (const auto& element : layout)
		{
			uint32_t attribIndex = m_VertexBufferIndex++;
			GLenum baseType = ShaderDataTypeToOpenGLBaseType(element.GetType());
			uint32_t count = element.GetComponentCount();
			uint32_t offset = static_cast<uint32_t>(element.GetOffset());

			switch (element.GetType())
			{
				case ShaderDataType::Float:
				case ShaderDataType::Float2:
				case ShaderDataType::Float3:
				case ShaderDataType::Float4: 
				{
					GLCall(glEnableVertexArrayAttrib(m_RendererID, attribIndex));
					GLCall(glVertexArrayAttribFormat(m_RendererID, attribIndex,
						count,
						baseType,
						element.GetNormalized() ? GL_TRUE : GL_FALSE,
						offset));
					break;
				}
				case ShaderDataType::Int:
				case ShaderDataType::Int2:
				case ShaderDataType::Int3:
				case ShaderDataType::Int4:
				case ShaderDataType::Bool: // Booleans should not be normalized
				{
					GLCall(glEnableVertexArrayAttrib(m_RendererID, attribIndex));
					GLCall(glVertexArrayAttribIFormat(m_RendererID, attribIndex,
						count,
						baseType,
						offset
					));
					break;
				}
				case ShaderDataType::Mat3:
				case ShaderDataType::Mat4: // Each row of the matrix is a different attribute (vertex).
				{
					// number of columns = number of matrix rows
					uint8_t cols = element.GetComponentCount();
					const uint32_t rowSize = cols * sizeof(float);
					for (uint8_t i = 0; i < cols; i++)
					{
						GLuint rowAttrib = attribIndex + i;
						GLCall(glEnableVertexArrayAttrib(m_RendererID, rowAttrib));
						GLCall(glVertexArrayAttribFormat(m_RendererID, rowAttrib,
							cols,
							GL_FLOAT,
							element.GetNormalized() ? GL_TRUE : GL_FALSE,
							offset + i * rowSize
						));
						GLCall(glVertexArrayAttribBinding(m_RendererID, rowAttrib, bindingIndex));
					}
					m_VertexBufferIndex += (cols - 1);
					break;
				}
			}

			GLCall(glVertexArrayAttribBinding(m_RendererID, attribIndex, bindingIndex));
		}
	}

	void VertexArray::SetElementBuffer(const std::shared_ptr<ElementBuffer>& elementBuffer)
	{
		// Since OpenGL 4.5 DSA
		GLCall(glVertexArrayElementBuffer(m_RendererID, elementBuffer->GetRendererID()));
		m_ElementBuffer = elementBuffer;
	}
}