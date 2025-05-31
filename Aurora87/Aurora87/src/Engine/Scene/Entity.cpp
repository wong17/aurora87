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

		// C�lculo de offsets en el UBO
		uint32_t alignedStride = modelUniformBuffer.GetAlignedStride();
		// Desplazamiento base para esta entidad: �ndice * tama�o alineado de cada bloque
		uint32_t baseOffset = entityIndex * alignedStride;
		// Desplazamiento interno donde se almacena el modelo dentro de cada bloque
		uint32_t modelOffset = modelUniformBuffer.GetLayout().GetElement("u_ModelMatrix").Offset;

		glm::mat4 matrix = GetWorldMatrix();

		modelUniformBuffer.Bind();
		// Subir solo la matriz de modelo a la regi�n de esta entidad en el UBO
		modelUniformBuffer.SetData(glm::value_ptr(matrix), sizeof(glm::mat4), baseOffset + modelOffset);
		// Vincular exclusivamente la regi�n de esta entidad (modelo, vista y proyecci�n) para que el shader lea �nicamente esos datos
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
		
		// C�lculo de offsets en el UBO
		uint32_t alignedStride = modelUniformBuffer.GetAlignedStride();
		// Desplazamiento base para esta entidad: �ndice * tama�o alineado de cada bloque
		uint32_t baseOffset = entityIndex * alignedStride;
		// Desplazamiento interno donde se almacena el modelo dentro de cada bloque
		uint32_t modelOffset = modelUniformBuffer.GetLayout().GetElement("u_ModelMatrix").Offset;

		glm::mat4 modelMat = GetWorldMatrix();

		modelUniformBuffer.Bind();
		// Subir solo la matriz de modelo a la regi�n de esta entidad en el UBO
		modelUniformBuffer.SetData(glm::value_ptr(modelMat), sizeof(glm::mat4), baseOffset + modelOffset);
		// Vincular exclusivamente la regi�n de esta entidad (modelo, vista y proyecci�n) para que el shader lea �nicamente esos datos
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