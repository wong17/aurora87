#include "ShadowMappingTestLayer.h"

namespace Test
{
	/*
	 * Perspective projections are most often used with spotlights and point lights, while
	 * orthographic projections are used for directional lights.
	 *
	 * To be able to properly view the depth values as we did with the orthographic
	 * projection you first want to transform the non-linear depth values to linear
	 */

	void ShadowMappingTestLayer::OnAttach()
	{
		// Texture Manager
		Engine::TextureSpecification spec;
		spec.SRGB = true;
		auto texture1 = Engine::TextureManager::Get().Load("res/textures/container.jpg", spec);
		auto texture2 = Engine::TextureManager::Get().Load("res/textures/container2.png", spec);
		auto texture3 = Engine::TextureManager::Get().Load("res/textures/wood.png", spec);

		// Shader Manager
		m_ShapesShader = Engine::ShaderManager::Get().Load(
			"res/shaders/test/shadow-mapping-instanced-vertex.glsl",
			"res/shaders/test/shadow-mapping-fragment.glsl");
		m_ShapesShader->Bind();
		m_ShapesShader->SetInt("u_Texture", 0);
		m_ShapesShader->Bind();
		m_ShapesShader->SetInt("u_ShadowMap", 1);

		m_PlaneShader = Engine::ShaderManager::Get().Load(
			"res/shaders/test/shadow-mapping-vertex.glsl", 
			"res/shaders/test/shadow-mapping-fragment.glsl");
		m_PlaneShader->Bind();
		m_PlaneShader->SetInt("u_Texture", 0);
		m_PlaneShader->Bind();
		m_PlaneShader->SetInt("u_ShadowMap", 1);

		m_TextShader = std::make_unique<Engine::Shader>(
			"res/shaders/text_rendering/text-rendering-vertex-shader.glsl",
			"res/shaders/text_rendering/text-rendering-fragment-shader.glsl"
		);

		// Instancing
		const unsigned int numBatches = 2;
		glm::vec3 areaMin(-40.0f, 5.0f, -40.0f);
		glm::vec3 areaMax(40.0f, 15.0f, 40.0f);
		auto batches = Engine::Utils::GenerateTransformBatches(numBatches, m_ShapeInstanceCount, areaMin, areaMax);

		// Skybox
		std::vector<std::string> faces
		{
			"res/textures/skybox/right.jpg",
			"res/textures/skybox/left.jpg",
			"res/textures/skybox/top.jpg",
			"res/textures/skybox/bottom.jpg",
			"res/textures/skybox/front.jpg",
			"res/textures/skybox/back.jpg"
		};
		m_SkyboxShader = std::make_shared<Engine::Shader>(
			"res/shaders/skybox/skybox-vertex-shader.glsl",
			"res/shaders/skybox/skybox-fragment-shader.glsl");
		m_Skybox = std::make_unique<Engine::Skybox>(faces, m_SkyboxShader);

		// Uniform Buffer
		m_CameraUniformBuffer = std::make_shared<Engine::UniformBuffer>(Engine::UniformBuffer::GetCameraLayout(), 0, 1u);
		m_ModelUniformBuffer = std::make_shared<Engine::UniformBuffer>(Engine::UniformBuffer::GetModelLayout(), 1);
		Engine::UniformBufferLayout textureBlockLayout =
		{
			{ Engine::ShaderDataType::Int, "u_UseGamma"   },
			{ Engine::ShaderDataType::Int, "u_UseShadows" }
		};
		m_TextureUniformBuffer = std::make_shared<Engine::UniformBuffer>(textureBlockLayout, 3);
		m_CameraUniformBuffer->BindToShader("Camera", { *m_ShapesShader, *m_PlaneShader });
		m_ModelUniformBuffer->BindToShader("Model", { *m_ShapesShader, *m_PlaneShader });
		m_TextureUniformBuffer->BindToShader("TextureBlock", { *m_ShapesShader, *m_PlaneShader });

		// Entity Manager
		m_EntityManager = std::make_unique<Engine::EntityManager>(*m_ModelUniformBuffer, *m_TextureUniformBuffer);
		m_TestCube = m_EntityManager->CreateEntity(Engine::PrimitiveFactory::CreateCube(texture1), m_ShapesShader, "TestCube");
		m_TestPlane = m_EntityManager->CreateEntity(Engine::PrimitiveFactory::CreatePlane(texture3), m_PlaneShader, "TestPlane");
		m_TestPentagon = m_EntityManager->CreateEntity(Engine::PrimitiveFactory::CreatePentagon(texture2), m_ShapesShader, "TestPentagon");

		// Scene Renderer
		m_SceneRenderer = std::make_shared<Engine::SceneRenderer>(*m_CameraUniformBuffer);
		// Transform System
		m_TransformSystem = std::make_shared<Engine::TransformSystem>(*m_EntityManager);

		Engine::TransformComponent planeTransform;
		planeTransform.Scale = { 2 * 50.0f, 1.0f, 2 * 50.0f };
		planeTransform.Position = { 0.0f, -0.5f, 0.0f };

		m_TransformSystem->SetEntityTransform(m_TestPlane, planeTransform);

		m_EntityManager->SetInstanceTransforms({
			{ m_TestCube, batches[0] },
			{ m_TestPentagon, batches[1] }
			});

		auto& app = Engine::Application::Get();

		// Orthographic Camera
		m_OrthographicCamera = std::make_shared<Engine::OrthographicCamera>(
			0.0f, static_cast<float>(app.GetWindow().GetWidth()),
			0.0f, static_cast<float>(app.GetWindow().GetHeight()));

		// Text Renderer
		m_TextRenderer = std::make_unique<Engine::TextRenderer>("res/fonts/BlackOpsOne-Regular.ttf", 32);
		m_TextShader->Bind();
		m_TextShader->SetMat4("u_ProjectionMatrix", m_OrthographicCamera->GetProjectionMatrix());

		// Shadow Shader
		m_DepthShader = Engine::ShaderManager::Get().Load(
			"res/shaders/test/shadow-mapping-depth-vertex.glsl",
			"res/shaders/test/shadow-mapping-depth-fragment.glsl");

		// Shadow Map
		m_OrthographicShadowCamera = std::make_shared<Engine::OrthographicShadowCamera>(-50.0f, 50.0f, -50.0f, 50.0f, 0.1f, 100.0f);
		m_OrthographicShadowCamera->SetLightParams(
			m_LightPosition, // Posición de la luz
			m_LightDirection // Dirección (hacia abajo)
		);
		m_ShadowMap = std::make_shared<Engine::ShadowMap>(m_OrthographicShadowCamera, 1024, 1024);
		Engine::g_EnableShadows = true;

		// Audio Manager
		m_AudioManager = std::make_shared<Engine::AudioManager>();
		m_AudioManager->LoadBuffer("Matador", "res/audio/El-matador.wav");
		m_CurrentAudioSource = m_AudioManager->AcquireSource("Matador");
		m_CurrentAudioSource->SetLooping(false);
		m_CurrentAudioSource->Play();

		// Desactivar el cursor para el movimiento de la cámara
		app.GetWindow().SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		app.GetRenderSettings().SetDepthTest(true);
		app.GetRenderSettings().SetCullFace(true);
		app.GetRenderSettings().SetAlphaBlending(true);
	}

	void ShadowMappingTestLayer::OnDetach()
	{
	}

	void ShadowMappingTestLayer::OnUpdate(float deltaTime)
	{
		auto& app = Engine::Application::Get();

		m_OrthographicShadowCamera->SetLightParams(m_LightPosition, m_LightDirection);

		// Pasamos el ProjectionMatrix * LightViewMatrix
		m_DepthShader->Bind();
		m_DepthShader->SetMat4("u_LightSpaceMatrix", m_ShadowMap->GetLightSpaceMatrix());
		
		// Depth pass
		m_ShadowMap->BeginDepthPass();
		// Dibujar todas las entidades pero sin texturas para generar el depth map
		m_SceneRenderer->RenderDepth(*m_EntityManager, *m_DepthShader);
		m_ShadowMap->EndDepthPass();

		// Scene pass
		auto* camera = static_cast<Engine::PerspectiveCamera*>(&app.GetCamera());
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection = camera->GetProjectionMatrix();

		// Actualizar la matriz de vista y proyección de todas las entidades
		m_SceneRenderer->SetViewProjection(view, projection);

		// 
		m_ShapesShader->Bind();
		m_ShadowMap->BindDepthTexture(1);
		m_ShapesShader->SetMat4("u_LightSpaceMatrix", m_ShadowMap->GetLightSpaceMatrix());
		m_ShapesShader->SetFloat3("u_LightPosition", m_LightPosition);
		m_ShapesShader->SetFloat3("u_ViewPosition", camera->GetPosition());

		m_PlaneShader->Bind();
		m_ShadowMap->BindDepthTexture(1);
		m_PlaneShader->SetMat4("u_LightSpaceMatrix", m_ShadowMap->GetLightSpaceMatrix());
		m_PlaneShader->SetFloat3("u_LightPosition", m_LightPosition);
		m_PlaneShader->SetFloat3("u_ViewPosition", camera->GetPosition());
		
		// Renderizar todas las entidades
		m_SceneRenderer->RenderAll(*m_EntityManager, true);

		// Dibujar el Skybox
		m_Skybox->Draw(view, projection);

		// Dibujar el texto
		m_TextShader->Bind();
		m_TextRenderer->RenderText(*m_TextShader, "Shadow Mapping", 10.0f, 10.0f, 1.0f, { 1.0f,1.0f,1.0f });

		// Audio Update
		if (m_CurrentAudioSource)
		{
			m_CurrentAudioSource->Update(deltaTime);
			if (m_CurrentAudioSource->GetState() == Engine::AudioSource::State::Stopped)
			{
				m_AudioManager->ReleaseSource(m_CurrentAudioSource);
				m_CurrentAudioSource.reset();
			}
		}
	}

	void ShadowMappingTestLayer::OnImGuiRender()
	{
		ImGui::Begin("Shadow Debug Panel");

		// Toggle wireframe mode
		if (ImGui::Button("Toggle Wireframe Mode")) 
		{
			m_WireframeMode = !m_WireframeMode;
			glPolygonMode(GL_FRONT_AND_BACK, m_WireframeMode ? GL_LINE : GL_FILL);
		}

		// Controles para la luz
		ImGui::SeparatorText("Light Settings");
		ImGui::DragFloat3("Light Position", glm::value_ptr(m_LightPosition), 1.0f, -100.0f, 500.0f);
		ImGui::DragFloat3("Light Direction", glm::value_ptr(m_LightDirection), 0.01f, -1.0f, 1.0f);

		// Controles para la cámara de sombras ortográfica
		if (m_OrthographicShadowCamera) 
		{
			ImGui::SeparatorText("Shadow Camera Settings");

			static float left = -50.0f, right = 50.0f, bottom = -50.0f, top = 50.0f;
			static float nearPlane = 0.1f, farPlane = 100.0f;

			ImGui::DragFloat("Left", &left, 1.0f, -200.0f, 200.0f);
			ImGui::DragFloat("Right", &right, 1.0f, -200.0f, 200.0f);
			ImGui::DragFloat("Bottom", &bottom, 1.0f, -200.0f, 200.0f);
			ImGui::DragFloat("Top", &top, 1.0f, -200.0f, 200.0f);
			ImGui::DragFloat("Near", &nearPlane, 0.1f, 0.1f, 50.0f);
			ImGui::DragFloat("Far", &farPlane, 1.0f, 10.0f, 500.0f);

			if (ImGui::Button("Update Shadow Camera")) 
			{
				m_OrthographicShadowCamera->SetProjection(left, right, bottom, top);
				m_OrthographicShadowCamera->SetNearFar(nearPlane, farPlane);
			}
		}

		// Debug del Depth Map
		ImGui::SeparatorText("Debug Tools");
		ImGui::Checkbox("Show Depth Map", &m_ShowDepthDebug);

		ImGui::End();
	}

	void ShadowMappingTestLayer::OnEvent(Engine::Event& event)
	{
		Engine::Application::Get().GetCamera().OnEvent(event);
		if (event.GetEventType() == Engine::EventType::MouseScrolled)
		{
			auto& mouseScrolledEvent = static_cast<Engine::MouseScrolledEvent&>(event);
			OnMouseScroll(mouseScrolledEvent);
		}
	}

	void ShadowMappingTestLayer::OnMouseScroll(Engine::MouseScrolledEvent& event)
	{
		auto* perspectiveCamera = static_cast<Engine::PerspectiveCamera*>(&Engine::Application::Get().GetCamera());

		glm::mat4 projection = perspectiveCamera->GetProjectionMatrix();

		// Actualizar la matriz de proyección en el shader
		m_CameraUniformBuffer->Bind();
		m_CameraUniformBuffer->SetData("u_ProjectionMatrix", glm::value_ptr(projection));

		m_Skybox->SetProjectionMatrix(projection);
	}
}