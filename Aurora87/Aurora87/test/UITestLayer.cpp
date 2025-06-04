#include "UITestLayer.h"

namespace Test
{
	void UITestLayer::OnAttach()
	{
		// Texture Manager
		Engine::TextureSpecification spec;
		spec.SRGB = true;
		m_playNormal = Engine::TextureManager::Get().Load("res/textures/Button/playNormal.png", spec);
		m_playHover = Engine::TextureManager::Get().Load("res/textures/Button/playHover.png", spec);
		m_exitNormal = Engine::TextureManager::Get().Load("res/textures/Button/exitNormal.png", spec);
		m_exitHover = Engine::TextureManager::Get().Load("res/textures/Button/exitHover.png", spec);
		
		// Shader Manager
		m_DepthShader = Engine::ShaderManager::Get().Load(
			"res/shaders/test/shadow-mapping-instanced-vertex.glsl",
			"res/shaders/test/shadow-mapping-fragment.glsl");
		m_DepthShader->Bind();
		m_DepthShader->SetInt("u_Texture", 0);
		m_DepthShader->Bind();
		m_DepthShader->SetInt("u_ShadowMap", 1);

		m_ButtonShader = Engine::ShaderManager::Get().Load(
			"res/shaders/test/ui-vertex-shader.glsl",
			"res/shaders/test/ui-fragment-shader.glsl");
		m_ButtonShader->Bind();
		m_ButtonShader->SetInt("u_Texture", 0);
		m_ButtonShader->Bind();
		m_ButtonShader->SetInt("u_ShadowMap", 1);

		// Uniform Buffer
		m_CameraUniformBuffer = std::make_shared<Engine::UniformBuffer>(Engine::UniformBuffer::GetCameraLayout(), 0, 1u);
		m_ModelUniformBuffer = std::make_shared<Engine::UniformBuffer>(Engine::UniformBuffer::GetModelLayout(), 1);
		Engine::UniformBufferLayout textureBlockLayout =
		{
			{ Engine::ShaderDataType::Int, "u_UseGamma"   },
			{ Engine::ShaderDataType::Int, "u_UseShadows" }
		};
		m_TextureUniformBuffer = std::make_shared<Engine::UniformBuffer>(textureBlockLayout, 3);

		// Skybox
		std::vector<std::string> faces
		{
			"res/textures/skybox_sp/top_space.png",
			"res/textures/skybox_sp/right_space.png",
			"res/textures/skybox_sp/left_space.png",
			"res/textures/skybox_sp/front_space.png",
			"res/textures/skybox_sp/back_space.png",
			"res/textures/skybox_sp/bottom_space.png"
		};
		m_SkyboxShader = std::make_shared<Engine::Shader>(
			"res/shaders/skybox/skybox-vertex-shader.glsl",
			"res/shaders/skybox/skybox-fragment-shader.glsl");
		m_Skybox = std::make_unique<Engine::Skybox>(faces, m_SkyboxShader);

	
		// Entity Manager
		m_EntityManager = std::make_unique<Engine::EntityManager>(*m_ModelUniformBuffer, *m_TextureUniformBuffer);

		// Trabsform System
		m_TransformSystem = std::make_shared<Engine::TransformSystem>(*m_EntityManager);

		// Entidad para Play
		m_TestPlayButton = m_EntityManager->CreateEntity(
			Engine::PrimitiveFactory::CreateRectButton(m_playNormal),
			m_ButtonShader,
			"PlayButton"
		);

		Engine::TransformComponent playTransform;
		playTransform.Position = glm::vec3(-1.0f, 0.0f, -180.0f);
		playTransform.Scale = { 2 * 50.0f, 2 * 20.0f, 2 * 10.0f };
		playTransform.Rotation = { 90.0f, 0.0f, 0.0f };
		m_TransformSystem->SetEntityTransform(m_TestPlayButton, playTransform);

		// Entidad para Exit
		m_TestExitButton = m_EntityManager->CreateEntity(
			Engine::PrimitiveFactory::CreateRectButton(m_exitNormal),
			m_ButtonShader,
			"ExitButton"
		);

		Engine::TransformComponent exitTransform;
		exitTransform.Position = glm::vec3(-1.0f, -25.0f, -180.0f);
		exitTransform.Scale = { 2 * 50.0f, 2 * 20.0f, 2 * 10.0f };
		exitTransform.Rotation = { 90.0f, 0.0f, 0.0f };
		m_TransformSystem->SetEntityTransform(m_TestExitButton, exitTransform);

		// Crea los botones
		m_PlayButton = std::make_unique<UI::Button3D>(m_TestPlayButton, m_playNormal, m_playHover);
		m_PlayButton->SetOnClick([this]() {
			// Corre la siguiente capa
			/*m_ShowButtons = false;
			auto newLayer = std::make_shared<Test::ShadowMappingTestLayer>();
			Engine::Application::Get().PushOverlay(newLayer);*/
			});

		m_ExitButton = std::make_unique<UI::Button3D>(m_TestExitButton, m_exitNormal, m_exitHover);
		m_ExitButton->SetOnClick([]() {
			Engine::Application::Get().Close();
			});

		// Scene Renderer
		m_SceneRenderer = std::make_shared<Engine::SceneRenderer>(*m_CameraUniformBuffer);
		
		 // Shadow Mapping
		auto& app = Engine::Application::Get();
		unsigned int width = static_cast<unsigned int>(app.GetWindow().GetWidth());
		unsigned int height = static_cast<unsigned int>(app.GetWindow().GetHeight());
		m_OrthographicShadowCamera = std::make_shared<Engine::OrthographicShadowCamera>(-50.0f, 50.0f, -50.0f, 50.0f, 0.1f, 100.0f);
		m_OrthographicShadowCamera->SetLightParams(m_LightPosition, m_LightDirection);
		m_ShadowMap = std::make_shared<Engine::ShadowMap>(m_OrthographicShadowCamera, width, height);
		
		// Limitar el movimiento de la camara
		auto* camera = static_cast<Engine::PerspectiveCamera*>(&app.GetCamera());
		camera->EnableKeyboard(false);
		camera->SetYawLimits(camera->GetYaw() - 10.0f, camera->GetYaw() + 10.0f);
		camera->SetPitchLimits(-10.0f, 10.0f);

		// Disable cursor
		app.GetWindow().SetInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		app.GetRenderSettings().SetDepthTest(true);
		app.GetRenderSettings().SetCullFace(true);
		app.GetRenderSettings().SetAlphaBlending(true);
	}

	void UITestLayer::OnDetach()
	{
	}

	void UITestLayer::OnUpdate(float deltaTime)
	{
		// Setup
		auto& app = Engine::Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		auto* camera = static_cast<Engine::PerspectiveCamera*>(&app.GetCamera());

		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection = camera->GetProjectionMatrix();
		glm::vec3 cameraPos = camera->GetPosition();

		// Actualiza la matriz de vista y proyeccion de todas las entidades
		m_CameraUniformBuffer->Bind();
		m_CameraUniformBuffer->SetData("u_ViewMatrix", glm::value_ptr(view));
		m_CameraUniformBuffer->SetData("u_ProjectionMatrix", glm::value_ptr(projection));

		m_OrthographicShadowCamera->SetLightParams(m_LightPosition, m_LightDirection);

		// Shadow Pass
		m_DepthShader->Bind();
		m_DepthShader->SetMat4("u_LightSpaceMatrix", m_ShadowMap->GetLightSpaceMatrix());

		m_ShadowMap->BeginDepthPass();
		m_SceneRenderer->RenderDepth(*m_EntityManager, *m_DepthShader);
		m_ShadowMap->EndDepthPass();

		// Scene Pass
		m_ButtonShader->Bind();
		m_ShadowMap->BindDepthTexture(1);
		m_ButtonShader->SetMat4("u_LightSpaceMatrix", m_ShadowMap->GetLightSpaceMatrix());
		m_ButtonShader->SetFloat3("u_LightPosition", m_LightPosition);
		m_ButtonShader->SetFloat3("u_ViewPosition", cameraPos);

		// Renderiza el button
		m_SceneRenderer->SetViewProjection(view, projection);
		m_SceneRenderer->RenderAll(*m_EntityManager, true);

		// Dibuja el Skybox
		m_Skybox->Draw(view, projection);

		// Button Update
		m_PlayButton->Update(view, projection, cameraPos, window);
		m_ExitButton->Update(view, projection, cameraPos, window);
	}

	void UITestLayer::OnImGuiRender()
	{
		ImGui::Begin("Shadow Debug Panel");

		if (ImGui::Button("Toggle Wireframe Mode"))
		{
			m_WireframeMode = !m_WireframeMode;
			glPolygonMode(GL_FRONT_AND_BACK, m_WireframeMode ? GL_LINE : GL_FILL);
		}

		ImGui::SeparatorText("Light Settings");
		ImGui::DragFloat3("Light Position", glm::value_ptr(m_LightPosition), 1.0f, -100.0f, 500.0f);
		ImGui::DragFloat3("Light Direction", glm::value_ptr(m_LightDirection), 0.01f, -1.0f, 1.0f);

		ImGui::SeparatorText("Debug Tools");
		ImGui::Checkbox("Show Depth Map", &m_ShowDepthDebug);

		ImGui::End();
	}

	void UITestLayer::OnEvent(Engine::Event& event)
	{
		Engine::Application::Get().GetCamera().OnEvent(event);
		if (event.GetEventType() == Engine::EventType::MouseScrolled)
		{
			auto& mouseScrolledEvent = static_cast<Engine::MouseScrolledEvent&>(event);
			OnMouseScroll(mouseScrolledEvent);
		}
	}

	void UITestLayer::OnMouseScroll(Engine::MouseScrolledEvent& event)
	{
		auto* perspectiveCamera = static_cast<Engine::PerspectiveCamera*>(&Engine::Application::Get().GetCamera());

		glm::mat4 projection = perspectiveCamera->GetProjectionMatrix();

		// Actualizar la matriz de proyeccion en el shader
		m_CameraUniformBuffer->Bind();
		m_CameraUniformBuffer->SetData("u_ProjectionMatrix", glm::value_ptr(projection));

		m_Skybox->SetProjectionMatrix(projection);
	}
}