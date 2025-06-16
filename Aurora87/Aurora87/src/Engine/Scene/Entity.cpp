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

		// Draw mesh/model without textures
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

		// Draw mesh/model without textures
		if (m_Model)
			m_Model->DrawInstancedDepth(depthShader, instanceCount);
		else if (m_Mesh)
			m_Mesh->DrawInstancedDepth(depthShader, instanceCount);

		for (auto& child : m_Childrens)
			child->DrawInstancedDepth(depthShader, instanceCount);
	}

	void Entity::Draw(UniformBuffer& textureBlockUniformBuffer, uint32_t entityIndex, bool bindTextures)
	{
		m_Shader->Bind();
		glm::mat4 matrix = GetWorldMatrix();
		m_Shader->SetMat4("u_ModelMatrix", matrix);

		if (m_Model)
			m_Model->Draw(*m_Shader, textureBlockUniformBuffer, entityIndex, bindTextures);
		else if (m_Mesh)
			m_Mesh->Draw(*m_Shader, textureBlockUniformBuffer, entityIndex, bindTextures);

		for (auto& child : m_Childrens)
			child->Draw(textureBlockUniformBuffer, entityIndex, bindTextures);
	}

	void Entity::Draw(
		bool bindTextures, 
		UniformBuffer& modelUniformBuffer, 
		UniformBuffer& textureBlockUniformBuffer,
		uint32_t entityIndex)
	{
		m_Shader->Bind();

		// Calculation of offsets in the UBO
		uint32_t alignedStride = modelUniformBuffer.GetAlignedStride();
		// Base offset for this entity: index * aligned size of each block
		uint32_t baseOffset = entityIndex * alignedStride;
		// Internal scrolling where the model is stored within each block
		uint32_t modelOffset = modelUniformBuffer.GetLayout().GetElement("u_ModelMatrix").Offset;

		glm::mat4 matrix = GetWorldMatrix();

		modelUniformBuffer.Bind();
		// Upload only the model matrix to the region of this entity in the UBO.
		modelUniformBuffer.SetData(glm::value_ptr(matrix), sizeof(glm::mat4), baseOffset + modelOffset);
		// Bind exclusively the region of this entity (model, view and projection) so that the shader reads only this data
		modelUniformBuffer.BindRange(baseOffset, alignedStride);

		if (m_Model) 
			m_Model->Draw(*m_Shader, textureBlockUniformBuffer, entityIndex, bindTextures);
		else if (m_Mesh)
			m_Mesh->Draw(*m_Shader, textureBlockUniformBuffer, entityIndex, bindTextures);

		for (auto& child : m_Childrens) 
			child->Draw(bindTextures, modelUniformBuffer, textureBlockUniformBuffer, entityIndex);
	}

	void Entity::DrawInstanced(UniformBuffer& textureBlockUniformBuffer, uint32_t entityIndex, bool bindTextures, uint32_t instanceCount)
	{
		m_Shader->Bind();

		glm::mat4 matrix = GetWorldMatrix();
		m_Shader->SetMat4("u_ModelMatrix", matrix);

		if (m_Model)
			m_Model->DrawInstanced(*m_Shader, textureBlockUniformBuffer, entityIndex, instanceCount, bindTextures);
		else if (m_Mesh)
			m_Mesh->DrawInstanced(*m_Shader, textureBlockUniformBuffer, entityIndex, instanceCount, bindTextures);

		for (auto& child : m_Childrens)
			child->DrawInstanced(textureBlockUniformBuffer, entityIndex, bindTextures, instanceCount);
	}

	void Entity::DrawInstanced(
		bool bindTextures, 
		UniformBuffer& modelUniformBuffer, 
		UniformBuffer& textureBlockUniformBuffer, 
		uint32_t entityIndex, 
		uint32_t instanceCount)
	{
		m_Shader->Bind();
		
		// Calculation of offsets in the UBO
		uint32_t alignedStride = modelUniformBuffer.GetAlignedStride();
		// Base offset for this entity: index * aligned size of each block
		uint32_t baseOffset = entityIndex * alignedStride;
		// Internal scrolling where the model is stored within each block
		uint32_t modelOffset = modelUniformBuffer.GetLayout().GetElement("u_ModelMatrix").Offset;

		glm::mat4 modelMat = GetWorldMatrix();

		modelUniformBuffer.Bind();
		// Upload only the model matrix to the region of this entity in the UBO.
		modelUniformBuffer.SetData(glm::value_ptr(modelMat), sizeof(glm::mat4), baseOffset + modelOffset);
		// Bind exclusively the region of this entity (model, view and projection) so that the shader reads only this data
		modelUniformBuffer.BindRange(baseOffset, alignedStride);

		if (m_Model) 
			m_Model->DrawInstanced(*m_Shader, textureBlockUniformBuffer, entityIndex, instanceCount, bindTextures);
		else if (m_Mesh)
			m_Mesh->DrawInstanced(*m_Shader, textureBlockUniformBuffer, entityIndex, instanceCount, bindTextures);

		for (auto& child : m_Childrens) 
			child->DrawInstanced(bindTextures, modelUniformBuffer, textureBlockUniformBuffer, entityIndex, instanceCount);
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