#pragma once

#include "Engine/Core/Application.h"
#include "Engine/Core/RenderSettings.h"
#include "Engine/Core/Layer.h"

#include "Engine/Events/Event.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Events/KeyEvent.h"

#include "Engine/OpenGL/Shader.h"
#include "Engine/OpenGL/CubemapTexture.h"
#include "Engine/OpenGL/ShaderManager.h"
#include "Engine/OpenGL/ShaderDataTypes.h"
#include "Engine/OpenGL/PerspectiveCamera.h"
#include "Engine/OpenGL/PerspectiveShadowCamera.h"
#include "Engine/OpenGL/OrthographicCamera.h"
#include "Engine/OpenGL/OrthographicShadowCamera.h"
#include "Engine/OpenGL/Model.h"
#include "Engine/OpenGL/ShadowMap.h"
#include "Engine/OpenGL/VertexArray.h"
#include "Engine/OpenGL/VertexBuffer.h"
#include "Engine/OpenGL/VertexBufferLayout.h"
#include "Engine/OpenGL/Texture.h"
#include "Engine/OpenGL/UniformBuffer.h"
#include "Engine/OpenGL/UniformBufferLayout.h"
#include "Engine/OpenGL/TextRenderer.h"
#include "Engine/OpenGL/Skybox.h"

#include "Engine/Scene/SceneRenderer.h"
#include "Engine/Scene/TransformSystem.h"
#include "Engine/Scene/TransformComponent.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Scene/EntityManager.h"
#include "Engine/Scene/GlobalSceneSettings.h"

#include "Engine/Meshes/Cube.h"
#include "Engine/Meshes/PrimitiveFactory.h"
#include "Engine/Meshes/PrimitiveMesh.h"

#include "Engine/Util/Utils.h"

#include "UI/Button3D.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <memory>
#include <vector>
#include <cmath>
#include <numbers>
#include <algorithm>
#include <map>
#include <string>
#include <filesystem>

namespace Test
{
	class UITestLayer : public Engine::Layer
	{
	public:
		UITestLayer() = default;
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(float deltaTime) override;
		void OnImGuiRender() override;
		void OnEvent(Engine::Event& event) override;
		void OnMouseScroll(Engine::MouseScrolledEvent& event);

	private:
		// Skybox
		std::unique_ptr<Engine::Skybox> m_Skybox = nullptr;
		std::shared_ptr<Engine::Shader> m_SkyboxShader = nullptr;

		// Shaders
		std::shared_ptr<Engine::Shader> m_DepthShader = nullptr;
		std::shared_ptr<Engine::Shader> m_ButtonShader = nullptr;

		// Managers
		std::shared_ptr<Engine::EntityManager> m_EntityManager = nullptr;
		std::shared_ptr<Engine::SceneRenderer> m_SceneRenderer = nullptr;
		std::shared_ptr<Engine::TransformSystem> m_TransformSystem = nullptr;

		// Uniform Buffer
		std::shared_ptr<Engine::UniformBuffer> m_CameraUniformBuffer;
		std::shared_ptr<Engine::UniformBuffer> m_ModelUniformBuffer;
		std::shared_ptr<Engine::UniformBuffer> m_TextureUniformBuffer = nullptr;

		// Shadow Mapping
		std::shared_ptr<Engine::ShadowMap> m_ShadowMap;
		std::shared_ptr<Engine::OrthographicShadowCamera> m_OrthographicShadowCamera = nullptr;

		// Button
		std::shared_ptr<Engine::Texture> m_playNormal;
		std::shared_ptr<Engine::Texture> m_playHover;
		std::shared_ptr<Engine::Texture> m_exitNormal;
		std::shared_ptr<Engine::Texture> m_exitHover;

		std::shared_ptr<Engine::Entity> m_TestPlayButton;
		std::shared_ptr<Engine::Entity> m_TestExitButton;

		std::unique_ptr<UI::Button3D> m_PlayButton;
		std::unique_ptr<UI::Button3D> m_ExitButton;

		// Light
		glm::vec3 m_LightPosition = { 0.0f, 50.0f, 0.0f };
		glm::vec3 m_LightDirection = { 0.0f, -1.0f, 0.0f };

		bool m_WireframeMode = false;
		bool m_ShowDepthDebug = true;
		//bool m_ShowButtons = true;
	};
}