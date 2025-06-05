#include "MainSceneTestLayer.h"

namespace Test
{
	void MainSceneTestLayer::OnAttach()
	{
		// Shader Manager
		m_MoonSurfaceShader = std::make_shared<Engine::Shader>(
			"res/shaders/model/s-m-vertex-shader.glsl", 
			"res/shaders/model/s-m-fragment-shader.glsl"
		);

		m_TextShader = std::make_unique<Engine::Shader>(
			"res/shaders/text_rendering/text-rendering-vertex-shader.glsl",
			"res/shaders/text_rendering/text-rendering-fragment-shader.glsl"
		);

		// Skybox
		std::vector<std::string> faces
		{
			"res/textures/skybox/Space_Right.png",
			"res/textures/skybox/Space_Left.png",
			"res/textures/skybox/Space_Top.png",
			"res/textures/skybox/Space_Bottom.png",
			"res/textures/skybox/Space_Front.png",
			"res/textures/skybox/Space_Back.png"
		};
		m_SkyboxShader = std::make_shared<Engine::Shader>(
			"res/shaders/skybox/skybox-vertex-shader.glsl",
			"res/shaders/skybox/skybox-fragment-shader.glsl");
		m_Skybox = std::make_unique<Engine::Skybox>(faces, m_SkyboxShader);

		// Global lights
		Engine::g_LightCounts.MaxDirectionalLights = 1;
		Engine::g_LightCounts.MaxPointLights = 0;
		Engine::g_LightCounts.MaxSpotLights = 0;

		// Uniform Buffer
		m_CameraUniformBuffer = std::make_shared<Engine::UniformBuffer>(Engine::UniformBuffer::GetCameraLayout(), Engine::BINDING_CAMERA, 1u);
		m_ModelUniformBuffer = std::make_shared<Engine::UniformBuffer>(Engine::UniformBuffer::GetModelLayout(), Engine::BINDING_MODEL);
		m_TextureUniformBuffer = std::make_shared<Engine::UniformBuffer>(Engine::UniformBuffer::GetTextureBlockLayout(), Engine::BINDING_TEXTUREBLOCK);
		m_LightUniformBuffer = std::make_shared<Engine::UniformBuffer>(
			Engine::UniformBuffer::GetLightBlockLayout(
				Engine::g_LightCounts.MaxDirectionalLights, 
				Engine::g_LightCounts.MaxPointLights, 
				Engine::g_LightCounts.MaxSpotLights), Engine::BINDING_LIGHTBLOCK, 1u);
		m_CameraUniformBuffer->BindToShader("Camera", { *m_MoonSurfaceShader });
		m_ModelUniformBuffer->BindToShader("Model", { *m_MoonSurfaceShader });
		m_TextureUniformBuffer->BindToShader("TextureBlock", { *m_MoonSurfaceShader });
		m_LightUniformBuffer->BindToShader("LightBlock", { *m_MoonSurfaceShader });

		// Entity Manager
		m_EntityManager = std::make_unique<Engine::EntityManager>(*m_ModelUniformBuffer, *m_TextureUniformBuffer);
		InitializeModels();

		// Scene Renderer
		m_SceneRenderer = std::make_shared<Engine::SceneRenderer>(*m_CameraUniformBuffer);
		// Transform System
		m_TransformSystem = std::make_shared<Engine::TransformSystem>(*m_EntityManager);

		// Light Manager
		m_LightManager = std::make_shared<Engine::LightManager>(
			m_LightUniformBuffer, 
			Engine::g_LightCounts.MaxDirectionalLights, 
			Engine::g_LightCounts.MaxPointLights, 
			Engine::g_LightCounts.MaxSpotLights);
		InitializeLights();

		auto& app = Engine::Application::Get();
		auto* camera = static_cast<Engine::PerspectiveCamera*>(&app.GetCamera());
		camera->SetFarClip(2000.0f);
		camera->SetPosition({ 0.0f, 100.0f, 0.0f });

		// Orthographic Camera
		m_OrthographicCamera = std::make_shared<Engine::OrthographicCamera>(
			0.0f, static_cast<float>(app.GetWindow().GetWidth()),
			0.0f, static_cast<float>(app.GetWindow().GetHeight()));

		// Text Renderer
		m_TextRenderer = std::make_unique<Engine::TextRenderer>("res/fonts/BlackOpsOne-Regular.ttf", 32);
		m_TextShader->Bind();
		m_TextShader->SetMat4("u_ProjectionMatrix", m_OrthographicCamera->GetProjectionMatrix());

		// Audio Manager
		m_AudioManager = std::make_shared<Engine::AudioManager>();

		// Desactivar el cursor para el movimiento de la cámara
		app.GetWindow().SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		app.GetRenderSettings().SetDepthTest(true);
		app.GetRenderSettings().SetCullFace(true);
		app.GetRenderSettings().SetAlphaBlending(true);
	}

	void MainSceneTestLayer::OnDetach()
	{
	}

	void MainSceneTestLayer::OnUpdate(float deltaTime)
	{
		auto& app = Engine::Application::Get();

		auto* camera = static_cast<Engine::PerspectiveCamera*>(&app.GetCamera());
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection = camera->GetProjectionMatrix();

		m_MoonSurfaceShader->Bind();
		m_MoonSurfaceShader->SetFloat3("u_ViewPosition", camera->GetPosition());

		m_SceneRenderer->SetViewProjection(view, projection);
		
		m_LightManager->UpdateSceneLights();

		m_SceneRenderer->RenderAll(*m_EntityManager, true);

		// Skybox
		m_Skybox->Draw(view, projection);

		// Dibujar el texto
		m_TextShader->Bind();
		m_TextRenderer->RenderText(*m_TextShader, "Aurora", 10.0f, 10.0f, 1.0f, { 1.0f,1.0f,1.0f });
	}

	void MainSceneTestLayer::OnImGuiRender()
	{
		ImGui::Begin("MainSceneTestLayer Debug Panel");

		// Toggle wireframe mode
		if (ImGui::Button("Toggle Wireframe Mode"))
		{
			m_WireframeMode = !m_WireframeMode;
			glPolygonMode(GL_FRONT_AND_BACK, m_WireframeMode ? GL_LINE : GL_FILL);
		}

		ImGui::End();
	}

	void MainSceneTestLayer::OnEvent(Engine::Event& event)
	{
		Engine::Application::Get().GetCamera().OnEvent(event);
		if (event.GetEventType() == Engine::EventType::MouseScrolled)
		{
			auto& mouseScrolledEvent = static_cast<Engine::MouseScrolledEvent&>(event);
			OnMouseScroll(mouseScrolledEvent);
		}
	}

	void MainSceneTestLayer::OnMouseScroll(Engine::MouseScrolledEvent& event)
	{
		auto* perspectiveCamera = static_cast<Engine::PerspectiveCamera*>(&Engine::Application::Get().GetCamera());

		glm::mat4 projection = perspectiveCamera->GetProjectionMatrix();

		// Actualizar la matriz de proyección en el shader
		m_CameraUniformBuffer->Bind();
		m_CameraUniformBuffer->SetData("u_ProjectionMatrix", glm::value_ptr(projection));

		m_Skybox->SetProjectionMatrix(projection);
	}

	void MainSceneTestLayer::InitializeLights()
	{
		m_LightManager->SetGlobalAmbient({ 0.5f, 0.5f, 0.5f });

		Engine::DirectionalLight dir(glm::vec3(-0.3f, 1.0f, -0.3f), glm::vec3(0.2f));

		m_LightManager->AddDirectionalLight(dir);
	}

	void MainSceneTestLayer::InitializeModels()
	{
		m_MoonSurface = m_EntityManager->CreateEntity(
			std::make_shared<Engine::Model>("D:\\Blender\\MoonSurface.glb"), m_MoonSurfaceShader, "MoonSurface");

		m_MoonSurface->Scale(glm::vec3(10.0f));
	}
}