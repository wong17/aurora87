#include "Entity.h"

namespace Engine
{
	Entity::Entity(
		std::shared_ptr<Mesh> mesh, 
		std::shared_ptr<Shader> shader,
		const std::string& name)
		: m_Mesh(mesh), m_Model(nullptr), m_Shader(shader), m_Name(name)
	{ }

	Entity::Entity(
		std::shared_ptr<Model> model, 
		std::shared_ptr<Shader> shader,
		const std::string& name)
		: m_Mesh(nullptr), m_Model(model), m_Shader(shader), m_Name(name)
	{ }

	void Entity::DrawDepth(Shader & depthShader)
	{
		depthShader.Bind();
		
		glm::mat4 model = GetWorldMatrix();
		depthShader.SetMat4("u_ModelMatrix", model);

		// Dibujar mesh/model sin texturas
		if (m_Model)
			m_Model->DrawDepth(depthShader);
		else if (m_Mesh)
			m_Mesh->DrawDepth(depthShader);

		for (auto& child : m_Childrens)
			child->DrawDepth(depthShader);
	}

	void Entity::DrawInstancedDepth(Shader& depthShader, uint32_t instanceCount)
	{
		depthShader.Bind();
		
		glm::mat4 model = GetWorldMatrix();
		depthShader.SetMat4("u_ModelMatrix", model);

		// Dibujar mesh/model sin texturas
		if (m_Model)
			m_Model->DrawInstancedDepth(depthShader, instanceCount);
		else if (m_Mesh)
			m_Mesh->DrawInstancedDepth(depthShader, instanceCount);

		for (auto& child : m_Childrens)
			child->DrawInstancedDepth(depthShader, instanceCount);
	}

	void Entity::Draw(bool bindTextures)
	{
		m_Shader->Bind();
		glm::mat4 matrix = GetWorldMatrix();
		m_Shader->SetMat4("u_ModelMatrix", matrix);

		if (m_Model)
			m_Model->Draw(*m_Shader, bindTextures);
		else if (m_Mesh)
			m_Mesh->Draw(*m_Shader, bindTextures);

		for (auto& child : m_Childrens)
			child->Draw(bindTextures);
	}

	void Entity::Draw(bool bindTextures, UniformBuffer& uniformBuffer, uint32_t entityIndex)
	{
		m_Shader->Bind();

		// Cálculo de offsets en el UBO
		uint32_t alignedStride = uniformBuffer.GetAlignedStride();
		// Desplazamiento base para esta entidad: índice * tamaño alineado de cada bloque
		uint32_t baseOffset = entityIndex * alignedStride;
		// Desplazamiento interno donde se almacena el modelo dentro de cada bloque
		uint32_t modelOffset = uniformBuffer.GetLayout().GetElement("u_ModelMatrix").Offset;

		glm::mat4 matrix = GetWorldMatrix();

		uniformBuffer.Bind();
		// Subir solo la matriz de modelo a la región de esta entidad en el UBO
		uniformBuffer.SetData(glm::value_ptr(matrix), sizeof(glm::mat4), baseOffset + modelOffset);
		// Vincular exclusivamente la región de esta entidad (modelo, vista y proyección) para que el shader lea únicamente esos datos
		uniformBuffer.BindRange(baseOffset, alignedStride);

		if (m_Model) 
			m_Model->Draw(*m_Shader, bindTextures);
		else if (m_Mesh)
			m_Mesh->Draw(*m_Shader, bindTextures);

		for (auto& child : m_Childrens) 
			child->Draw(bindTextures, uniformBuffer, entityIndex);
	}

	void Entity::DrawInstanced(bool bindTextures, uint32_t instanceCount)
	{
		m_Shader->Bind();

		glm::mat4 matrix = GetWorldMatrix();
		m_Shader->SetMat4("u_ModelMatrix", matrix);

		if (m_Model)
			m_Model->DrawInstanced(*m_Shader, instanceCount, bindTextures);
		else if (m_Mesh)
			m_Mesh->DrawInstanced(*m_Shader, instanceCount, bindTextures);

		for (auto& child : m_Childrens)
			child->DrawInstanced(bindTextures, instanceCount);
	}

	void Entity::DrawInstanced(bool bindTextures, UniformBuffer& uniformBuffer, uint32_t entityIndex, uint32_t instanceCount)
	{
		m_Shader->Bind();
		
		// Cálculo de offsets en el UBO
		uint32_t alignedStride = uniformBuffer.GetAlignedStride();
		// Desplazamiento base para esta entidad: índice * tamaño alineado de cada bloque
		uint32_t baseOffset = entityIndex * alignedStride;
		// Desplazamiento interno donde se almacena el modelo dentro de cada bloque
		uint32_t modelOffset = uniformBuffer.GetLayout().GetElement("u_ModelMatrix").Offset;

		glm::mat4 modelMat = GetWorldMatrix();

		uniformBuffer.Bind();
		// Subir solo la matriz de modelo a la región de esta entidad en el UBO
		uniformBuffer.SetData(glm::value_ptr(modelMat), sizeof(glm::mat4), baseOffset + modelOffset);
		// Vincular exclusivamente la región de esta entidad (modelo, vista y proyección) para que el shader lea únicamente esos datos
		uniformBuffer.BindRange(baseOffset, alignedStride);

		if (m_Model) 
			m_Model->DrawInstanced(*m_Shader, instanceCount, bindTextures);
		else if (m_Mesh)
			m_Mesh->DrawInstanced(*m_Shader, instanceCount, bindTextures);

		for (auto& child : m_Childrens) 
			child->DrawInstanced(bindTextures, uniformBuffer, entityIndex, instanceCount);
	}

	std::shared_ptr<Entity> Entity::AddChild(std::shared_ptr<Model> childModel, std::shared_ptr<Shader> shader, const std::string& name)
	{
		auto child = std::make_shared<Entity>(std::move(childModel), shader, name);
		m_Childrens.push_back(child);
		return child;
	}

	void Entity::RemoveChild(const std::shared_ptr<Entity>& child)
	{
		auto it = std::find(m_Childrens.begin(), m_Childrens.end(), child);
		if (it != m_Childrens.end())
			m_Childrens.erase(it);
	}
}