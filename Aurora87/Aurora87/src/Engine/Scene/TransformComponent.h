#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Engine
{
	struct TransformComponent
	{
		glm::vec3 Position{ 0.0f };
		glm::vec3 Rotation{ 0.0f }; // Euler angles in degrees
		glm::vec3 Scale{ 1.0f };

		glm::mat4 GetLocalMatrix() const 
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), Position);
			model = glm::rotate(model, glm::radians(Rotation.x), { 1,0,0 });
			model = glm::rotate(model, glm::radians(Rotation.y), { 0,1,0 });
			model = glm::rotate(model, glm::radians(Rotation.z), { 0,0,1 });
			model = glm::scale(model, Scale);
			return model;
		}

		void Translate(const glm::vec3& delta) { Position += delta; }
		void Rotate(const glm::vec3& eulerDelta) { Rotation += eulerDelta; }
		void SetPosition(const glm::vec3& p) { Position = p; }
		void SetRotation(const glm::vec3& r) { Rotation = r; }
		void SetScale(const glm::vec3& s) { Scale = s; }
	};
}