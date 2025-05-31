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
#include "Engine/Scene/Entity.h"
#include "Engine/Scene/EntityManager.h"
#include "Engine/Meshes/Cube.h"
#include "Engine/Meshes/PrimitiveFactory.h"
#include "Engine/Meshes/PrimitiveMesh.h"
#include "Engine/Util/Utils.h"

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
	class SimpleGammaCorrectionTestLayer : public Engine::Layer
	{
	public:
		SimpleGammaCorrectionTestLayer() = default;
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
		std::shared_ptr<Engine::Shader> m_ShapesShader = nullptr;
		std::shared_ptr<Engine::Shader> m_PlaneShader = nullptr;
		std::shared_ptr<Engine::Entity> m_TestCube, m_TestPentagon, m_TestPlane;
		unsigned int m_ShapeInstanceCount = 100;

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
		std::shared_ptr<Engine::OrthographicCamera> m_OrthographicCamera = nullptr;
		std::shared_ptr<Engine::Shader> m_TextShader;

		// Light
		glm::vec3 m_LightPosition = { 0.0f, 0.0f, 0.0f };

		bool m_WireframeMode = false;
	};
}