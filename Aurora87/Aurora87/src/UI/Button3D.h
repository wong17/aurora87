#pragma once

#include "Engine/Core/DebugGL.h"
#include "Engine/Core/Application.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Scene/TransformComponent.h"
#include "Engine/OpenGL/Mesh.h"
#include "Engine/OpenGL/Texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <memory>
#include <functional>

namespace UI
{
	class Button3D
	{
	public:
		Button3D(std::shared_ptr<Engine::Entity> entity,
			std::shared_ptr<Engine::Texture> normalTex,
			std::shared_ptr<Engine::Texture> hoverTex);

		// Setter para callback onClick
		void SetOnClick(std::function<void()> callback) { m_OnClick = callback; }

		void Update(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos, GLFWwindow* window);

	private:
		std::shared_ptr<Engine::Entity> m_Entity = nullptr;
		Engine::TextureData m_NormalTextureData;
		Engine::TextureData m_HoverTextureData;

		bool m_MousePressedLastFrame = false;
		bool m_IsMouseOver = false;
		int m_HoverCounter = 1;

		std::function<void()> m_OnClick = nullptr;
	};
}