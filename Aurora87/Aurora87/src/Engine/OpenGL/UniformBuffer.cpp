#include "UniformBuffer.h"

namespace Engine
{
	UniformBuffer::UniformBuffer(const UniformBufferLayout& layout, uint32_t bindingPoint, uint32_t blockCount)
		: m_Layout(layout), m_BindingPoint(bindingPoint), m_BlockCount(blockCount)
	{
		// Calcular el stride en "crudo" (suma de tamaños de elementos)
		m_Stride = m_Layout.GetStride();

		// Obtener la alineación mínima requerida por GPU
		GLint alignment = 0;
		glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &alignment);
		// Ajustar 'm_Stride' al siguiente múltiplo de 'alignment' (alinear hacia arriba)
		m_AlignedStride = ((m_Stride + alignment - 1) / alignment) * alignment;

		GLint maxUniformBlockSize = 0;
		glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBlockSize);
		// Validamos que el stride individual no sea mayor al bloque permitido
		if (m_AlignedStride > static_cast<uint32_t>(maxUniformBlockSize)) {
			throw std::runtime_error(
				"UniformBuffer::UniformBuffer: El layout excede el tamanio maximo permitido por la GPU (" +
				std::to_string(m_AlignedStride) + " > " + std::to_string(maxUniformBlockSize) + " bytes)"
			);
		}

		// Validar que el tamaño total del buffer tampoco exceda el maximo
		GLsizeiptr totalSize = static_cast<GLsizeiptr>(m_AlignedStride) * m_BlockCount;
		if (totalSize > static_cast<GLsizeiptr>(maxUniformBlockSize)) {
			throw std::runtime_error(
				"UniformBuffer::UniformBuffer: El tamanio total del buffer excede el maximo permitido (" +
				std::to_string(totalSize) + " > " + std::to_string(maxUniformBlockSize) + " bytes)"
			);
		}
		
		// Crear buffer y reservar almacenamiento inmutable
		glCreateBuffers(1, &m_RendererID);
		GLCall(glNamedBufferStorage(m_RendererID, totalSize, nullptr, GL_DYNAMIC_STORAGE_BIT));
		// Enlazar el buffer al punto de enlace para usarlo en el shader
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
	 * Este método vincula al shader solo una subregión del UBO, definida por 'offset' y 'size'.
	 * 'offset': posición en bytes donde comienza el bloque de una entidad
	 * 'size': tamaño de bytes del bloque (normalmente 'alignedStride')
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
		// Obtener el índice del bloque uniforme en el shader
		GLuint blockIndex = glGetUniformBlockIndex(shader.GetRendererID(), blockName.c_str());
		if (blockIndex == GL_INVALID_INDEX) 
		{
			std::cerr << "UniformBuffer::BindToShader: block '" << blockName << "' no existe en shader '" << shader.GetName() << "'\n";
			return;
		}
		// Asociar el bloque uniforme al punto de enlace
		GLCall(glUniformBlockBinding(shader.GetRendererID(), blockIndex, bindingPoint));
		// Enlazar el Uniform Buffer al punto de enlace
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
				std::cerr << "UniformBuffer::BindToShader: block '"	<< blockName << "' no existe en shader '" << shader.GetName() << "'\n";
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
		
		// Guardamos el tamaño viejo para copiar
		GLsizeiptr oldTotalSize = static_cast<GLsizeiptr>(m_AlignedStride) * m_BlockCount;
		GLsizeiptr newTotalSize = static_cast<GLsizeiptr>(m_AlignedStride) * blockCount;

		// Crear el buffer nuevo
		GLuint newBuffer;
		glCreateBuffers(1, &newBuffer);

		// Asignar almacenamiento al buffer nuevo (permitiendo luego glCopyNamedBufferSubData)
		GLCall(glNamedBufferStorage(newBuffer,	newTotalSize, nullptr, GL_DYNAMIC_STORAGE_BIT));

		// Copiar la parte vieja al nuevo buffer
		GLCall(glCopyNamedBufferSubData(
			m_RendererID,   // srcBuffer
			newBuffer,      // dstBuffer
			0,              // readOffset
			0,              // writeOffset
			oldTotalSize    // size to copy
		));

		// Liberamos el buffer viejo
		glDeleteBuffers(1, &m_RendererID);

		// Guardamos el ID del nuevo buffer
		m_RendererID = newBuffer;
		m_BlockCount = blockCount;

		// Enlazamos el nuevo buffer al punto de enlace
		GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, m_BindingPoint, m_RendererID));
	}
}