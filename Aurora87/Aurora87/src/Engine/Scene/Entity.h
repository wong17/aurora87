#pragma once

#include "Engine/Meshes/PrimitiveMesh.h"
#include "Engine/OpenGL/Model.h"
#include "Engine/Scene/TransformComponent.h"
#include "Engine/OpenGL/Shader.h"
#include "Engine/OpenGL/UniformBuffer.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <memory>
#include <vector>
#include <cstdint>
#include <algorithm>

namespace Engine
{
	class Entity
	{
	public:
		Entity(
			std::shared_ptr<Mesh> mesh, 
			std::shared_ptr<Shader> shader, 
			const std::string& name = "", 
			const TransformComponent& transform = TransformComponent());
		Entity(
			std::shared_ptr<PrimitiveMesh> primitiveMesh, 
			std::shared_ptr<Shader> shader, 
			const std::string& name = "", 
			const TransformComponent& transform = TransformComponent());
		Entity(
			std::shared_ptr<Model> model, 
			std::shared_ptr<Shader> shader, 
			const std::string& name = "", 
			const TransformComponent& transform = TransformComponent());

		void DrawDepth(Shader& depthShader);
		void DrawInstancedDepth(Shader& depthShader, uint32_t instanceCount = 0);

		void Draw(bool bindTextures);
		void Draw(bool bindTextures, UniformBuffer& uniformBuffer, uint32_t entityIndex);
		void DrawInstanced(bool bindTextures, uint32_t instanceCount = 0);
		void DrawInstanced(bool bindTextures, UniformBuffer& uniformBuffer, uint32_t entityIndex, uint32_t instanceCount = 0);

		std::shared_ptr<Entity> AddChild(std::shared_ptr<Model> childModel, std::shared_ptr<Shader> shader, const std::string& name);
		void RemoveChild(const std::shared_ptr<Entity>& child);
		const std::vector<std::shared_ptr<Entity>>& GetChildrens() const { return m_Childrens; }

		const std::string& GetName() const { return m_Name; }
		TransformComponent& GetTransformComponent() { return m_TransformComponent; }
		std::shared_ptr<Model> GetModel() const { return m_Model; }
		std::shared_ptr<Mesh> GetMesh() const { return m_Mesh; }

		void SetShader(std::shared_ptr<Shader> shader) { m_Shader = shader; }
		void SetTransform(const TransformComponent& transform) { m_TransformComponent = transform; }
		void Translate(const glm::vec3& vector) { m_TransformComponent.Translate(vector); }
		void Rotate(const glm::vec3& vector) { m_TransformComponent.Rotate(vector); }
		void Scale(const glm::vec3& vector) { m_TransformComponent.Scale *= vector; }

		using EntityID = uint64_t;
		EntityID GetID() const { return m_ID; }

	private:
		std::string m_Name;
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<Model> m_Model = nullptr;
		std::shared_ptr<Mesh> m_Mesh = nullptr;
		TransformComponent m_TransformComponent;
		std::shared_ptr<Entity> m_Parent = nullptr;
		std::vector<std::shared_ptr<Entity>> m_Childrens;

		glm::mat4 GetWorldMatrix() const 
		{
			glm::mat4 local = m_TransformComponent.GetLocalMatrix();
			if (m_Parent != nullptr)
				return m_Parent->GetWorldMatrix() * local;
			return local;
		}

		friend class EntityManager;
		EntityID m_ID = 0;
	};
}