#pragma once

#include "Engine/Core/Application.h"
#include "Engine/Core/RenderSettings.h"
#include "Engine/Core/Layer.h"

#include "Engine/Events/Event.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Events/KeyEvent.h"

#include "Engine/OpenGL/Shader.h"
#include "Engine/OpenGL/ShaderManager.h"
#include "Engine/OpenGL/ShaderDataTypes.h"
#include "Engine/OpenGL/PerspectiveCamera.h"
#include "Engine/OpenGL/OrthographicCamera.h"
#include "Engine/OpenGL/Model.h"
#include "Engine/OpenGL/VertexArray.h"
#include "Engine/OpenGL/VertexBuffer.h"
#include "Engine/OpenGL/VertexBufferLayout.h"
#include "Engine/OpenGL/Texture.h"
#include "Engine/OpenGL/CubemapTexture.h"
#include "Engine/OpenGL/UniformBuffer.h"
#include "Engine/OpenGL/UniformBufferLayout.h"
#include "Engine/OpenGL/TextRenderer.h"
#include "Engine/OpenGL/Skybox.h"

#include "Engine/Scene/SceneRenderer.h"
#include "Engine/Scene/TransformSystem.h"

#include "Engine/Util/Utils.h"

#include "Engine/Scene/Entity.h"
#include "Engine/Scene/EntityManager.h"

#include "Engine/Meshes/Cube.h"
#include "Engine/Meshes/PrimitiveFactory.h"
#include "Engine/Meshes/PrimitiveMesh.h"

#include "Game/AIEnviroment/Base/BaseGameEntity.h"
#include "Game/AIEnviroment/Base/GameEntityManager.h"
#include "Game/AIEnviroment/Player/Player.h"
#include "Game/AIEnviroment/DumbZombie/DumbZombie.h"
#include "Game/AIEnviroment/GameLogic/CameraGame.h"

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

namespace Test {
	class AIEntitiesTestLayer : public Engine::Layer
	{
	public:
		AIEntitiesTestLayer() = default;
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

		// Figuras de prueba
		std::shared_ptr<Engine::Entity> m_TestCube, m_TestPyramid, m_TestPentagon, m_TestHexagon, m_TestIcosphere, m_Model;
		unsigned int m_ShapeInstanceCount = 1000;
		float m_Angle = 0.0f;
		float m_AngularSpeed = glm::radians(90.0f);
		float m_Radius = 3.0f;

		// Managers
		std::shared_ptr<Engine::EntityManager> m_EntityManager = nullptr;
		std::shared_ptr<Engine::SceneRenderer> m_SceneRenderer = nullptr;
		std::shared_ptr<Engine::TransformSystem> m_TransformSystem = nullptr;

		// Uniform Buffer
		std::shared_ptr<Engine::UniformBuffer> m_CameraUniformBuffer;
		std::shared_ptr<Engine::UniformBuffer> m_ModelUniformBuffer;
		std::shared_ptr<Engine::UniformBuffer> m_TextureUniformBuffer = nullptr;

		// Text Renderer
		std::shared_ptr<Engine::TextRenderer> m_TextRenderer = nullptr;
		 Engine::Shader* m_ShaderRenderer;
		std::shared_ptr<Engine::CameraGame> m_GameCamera = nullptr;
		std::shared_ptr<Engine::Shader> m_TextShader;

		// Player
		Player* m_Player = nullptr;

		//Zombie
		DumbZombie* m_DumbZombie = nullptr;
		bool m_WireframeMode = false;
	};
}