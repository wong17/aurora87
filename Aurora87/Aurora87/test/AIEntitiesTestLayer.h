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
#include "Engine/Scene/SceneRenderer.h"
#include "Engine/Scene/TransformSystem.h"
#include "Engine/OpenGL/Skybox.h"
#include "Engine/Util/Utils.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Scene/EntityManager.h"
#include "Engine/Meshes/Cube.h"
#include "Engine/Meshes/PrimitiveFactory.h"
#include "Engine/Meshes/PrimitiveMesh.h"
#include "AIEnviroment/Base/BaseGameEntity.h"
#include "AIEnviroment/Base/GameEntityManager.h"
#include "AIEnviroment/Player/Player.h"

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
	};

}