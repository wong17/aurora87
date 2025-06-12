#include "MainSceneTestLayer.h"

namespace Test
{
	void MainSceneTestLayer::OnAttach()
	{
		// Shader Manager
		auto& shaderManager = Engine::ShaderManager::Get();
		m_MoonSurfaceShader = shaderManager.Load(
			"res/shaders/model/s-m-vertex-shader.glsl", 
			"res/shaders/model/s-m-fragment-shader.glsl");

		m_TextShader = shaderManager.Load(
			"res/shaders/text_rendering/text-rendering-vertex-shader.glsl", 
			"res/shaders/text_rendering/text-rendering-fragment-shader.glsl");

		m_SkyboxShader = shaderManager.Load(
			"res/shaders/skybox/skybox-vertex-shader.glsl", 
			"res/shaders/skybox/skybox-fragment-shader.glsl");

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
		m_Skybox = std::make_unique<Engine::Skybox>(faces, m_SkyboxShader);

		// Global lights
		Engine::g_LightCounts.MaxDirectionalLights = 1;
		Engine::g_LightCounts.MaxPointLights = 1;
		Engine::g_LightCounts.MaxSpotLights = 1;

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
		m_LightAnimationManager = std::make_shared<Engine::LightAnimationManager>();
		InitializeLights();

		m_DirLights.reserve(Engine::g_LightCounts.MaxDirectionalLights);
		m_PointLights.reserve(Engine::g_LightCounts.MaxPointLights);
		m_SpotLights.reserve(Engine::g_LightCounts.MaxSpotLights);

		auto& app = Engine::Application::Get();
		auto* camera = static_cast<Engine::PerspectiveCamera*>(&app.GetCamera());
		camera->SetFarClip(5000.0f);
		camera->SetMovementSpeed(1000.0f);
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

		m_IsLoaded = true;
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

		m_LightAnimationManager->Update(deltaTime);
		
		m_LightManager->UpdateSceneLights();

		m_SceneRenderer->RenderAll(*m_EntityManager, true);

		// Skybox
		m_Skybox->Draw(view, projection);

		// Dibujar el texto
		m_TextShader->Bind();
		m_TextRenderer->RenderText(*m_TextShader, "Aurora 87", 10.0f, 10.0f, 1.0f, { 1.0f,1.0f,1.0f });
	}

	void MainSceneTestLayer::OnImGuiRender()
	{
		// Toggle wireframe mode
		ImGui::Begin("MainSceneTestLayer Debug Panel");
		if (ImGui::Button("Toggle Wireframe Mode"))
		{
			m_WireframeMode = !m_WireframeMode;
			glPolygonMode(GL_FRONT_AND_BACK, m_WireframeMode ? GL_LINE : GL_FILL);
		}
		ImGui::End();

		ImGui::Begin("Lights");
		// Global Ambient Color
		static glm::vec3 globalAmb = m_LightManager->GetGlobalAmbientColor();
		if (ImGui::ColorEdit3("##GlobalAmbientEdit", &globalAmb.x))
			m_LightManager->SetGlobalAmbient(globalAmb);

		if (ImGui::TreeNode("Global Ambient Color"))
		{
			if (ImGui::ColorPicker3("##GlobalAmbientPicker", &globalAmb.x, ImGuiColorEditFlags_NoInputs))
				m_LightManager->SetGlobalAmbient(globalAmb);
			ImGui::TreePop();
		}

		// Directional Lights
		for (int i = 0; i < static_cast<int>(m_DirLights.size()); ++i)
		{
			ImGui::PushID(i);
			ImGui::Text("Dir Light %d", i);
			bool changed = false;
			changed |= ImGui::DragFloat3("Direction", &m_DirLights[i].Direction.x, 0.01f, -1.0f, 1.0f);
			changed |= ImGui::ColorEdit3("Diffuse", &m_DirLights[i].Diffuse.x);
			if (changed)
				m_LightManager->SetDirectionalLight(i, m_DirLights[i]);
			ImGui::PopID();
		}

		ImGui::Separator();

		// Point Lights
		for (int i = 0; i < static_cast<int>(m_PointLights.size()); ++i)
		{
			ImGui::PushID(100 + i);
			ImGui::Text("Point Light %d", i);
			bool changed = false;
			changed |= ImGui::DragFloat3("Position", &m_PointLights[i].Position.x, 10.0f, -5000.0f, 5000.0f);
			changed |= ImGui::DragFloat("Constant", &m_PointLights[i].Constant, 0.01f, 0.0f, 10.0f);
			changed |= ImGui::DragFloat("Linear", &m_PointLights[i].Linear, 0.001f, 0.0f, 1.0f);
			changed |= ImGui::DragFloat("Quadratic", &m_PointLights[i].Quadratic, 0.0001f, 0.0f, 1.0f);
			changed |= ImGui::ColorEdit3("Diffuse", &m_PointLights[i].Diffuse.x);
			if (changed)
				m_LightManager->SetPointLight(i, m_PointLights[i]);
			ImGui::PopID();
		}

		ImGui::Separator();

		// Spot Lights
		for (int i = 0; i < static_cast<int>(m_SpotLights.size()); ++i)
		{
			ImGui::PushID(200 + i);
			ImGui::Text("Spot Light %d", i);
			bool changed = false;

			changed |= ImGui::DragFloat3("Position", &m_SpotLights[i].Position.x, 1.0f, -5000.0f, 5000.0f);

			changed |= ImGui::DragFloat3("Direction", &m_SpotLights[i].Direction.x, 0.01f, -1.0f, 1.0f);

			float innerDeg = glm::degrees(acos(glm::clamp(m_SpotLights[i].CutOff, -1.0f, 1.0f)));
			float outerDeg = glm::degrees(acos(glm::clamp(m_SpotLights[i].OuterCutOff, -1.0f, 1.0f)));
			if (ImGui::DragFloat("Inner Angle (deg)", &innerDeg, 0.1f, 0.0f, 90.0f))
			{
				innerDeg = glm::clamp(innerDeg, 0.0f, outerDeg);
				m_SpotLights[i].CutOff = glm::cos(glm::radians(innerDeg));
				changed = true;
			}
			if (ImGui::DragFloat("Outer Angle (deg)", &outerDeg, 0.1f, 0.0f, 90.0f))
			{
				outerDeg = glm::clamp(outerDeg, innerDeg, 90.0f);
				m_SpotLights[i].OuterCutOff = glm::cos(glm::radians(outerDeg));
				changed = true;
			}

			changed |= ImGui::DragFloat("Constant", &m_SpotLights[i].Constant, 0.01f, 0.0f, 10.0f);
			changed |= ImGui::DragFloat("Linear", &m_SpotLights[i].Linear, 0.001f, 0.0f, 1.0f);
			changed |= ImGui::DragFloat("Quadratic", &m_SpotLights[i].Quadratic, 0.0001f, 0.0f, 1.0f);

			changed |= ImGui::ColorEdit3("Diffuse", &m_SpotLights[i].Diffuse.x);

			if (changed)
			{
				glm::vec3 d = glm::normalize(glm::vec3(m_SpotLights[i].Direction));
				m_SpotLights[i].Direction = glm::vec4(d, 0.0f);
				m_LightManager->SetSpotLight(i, m_SpotLights[i]);
			}

			ImGui::PopID();
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
		// Global ambient light
		m_LightManager->SetGlobalAmbient(glm::vec3(0.005f));

		// Directional lights
		Engine::DirectionalLight dir(
			glm::vec3(-0.1f, 1.0f, 1.0f),
			50, 50, 50,
			111, 105, 105
		);
		m_DirLights.clear();
		m_DirLights.emplace_back(dir);

		for (size_t i = 0; i < m_DirLights.size(); ++i)
			m_LightManager->AddDirectionalLight(m_DirLights[i]);

		// Point lights
		Engine::PointLight point(
			glm::vec3(290.0f, 95.0f, 0.0f)
		);
		point.Linear = 0.045f;
		point.Quadratic = 0.0075f;
		m_PointLights.clear();
		m_PointLights.emplace_back(point);

		for (size_t i = 0; i < m_PointLights.size(); ++i)
			m_LightManager->AddPointLight(m_PointLights[i]);

		size_t index = 0;
		auto setterColor = [this, index](const glm::vec3& col) 
			{
			Engine::PointLight pl = m_PointLights[index];
			pl.Diffuse = glm::vec4(col, 0.0f);
			m_PointLights[index] = pl;
			m_LightManager->SetPointLight(static_cast<int>(index), pl);
			};
		float velocidad = 0.1f;
		auto colorTween = std::make_shared<Engine::ColorCycleTween>(velocidad, setterColor);

		auto clip = std::make_shared<Engine::LightAnimationClip>();
		clip->Tweens.push_back(colorTween);
		m_LightAnimationManager->AddClip(clip);

		// Spot lights
		m_SpotLights.clear();
		{
			glm::vec3 pos = { -0.0f, 80.0f, 0.0f };
			glm::vec3 dir = { 0.0f, -1.0f, 0.0f};
			Engine::SpotLight spot(pos, dir, 70.0f, 71.0f, 0.0f, 0.0f, 0.01f);
			spot.Ambient = glm::vec4(glm::vec3(1.0f), 0.0f);
			spot.Diffuse = glm::vec4(1.5f);
			m_SpotLights.push_back(spot);
		}

		for (size_t i = 0; i < m_SpotLights.size(); ++i)
			m_LightManager->AddSpotLight(m_SpotLights[i]);

		{
			int spotlightIndex = 0;

			auto getter = [this, spotlightIndex]() {
				return m_LightManager->GetSpotLight(spotlightIndex);
				};
			auto setter = [this, spotlightIndex](const Engine::SpotLight& spot) {
				m_LightManager->SetSpotLight(spotlightIndex, spot);
				};

			auto flickerTween = std::make_shared<Engine::FlickerTween<Engine::SpotLight>>(
				getter, setter, 0.2f
			);

			auto clip = std::make_shared<Engine::LightAnimationClip>();
			clip->Tweens.push_back(flickerTween);
			m_LightAnimationManager->AddClip(clip);
		}
	}

	void MainSceneTestLayer::InitializeModels()
	{
		m_MoonSurface = m_EntityManager->CreateEntity(
			std::make_shared<Engine::Model>("res/models/MoonSurface.glb"), m_MoonSurfaceShader, "MoonSurface");
		
		m_Earth = m_EntityManager->CreateEntity(
			std::make_shared<Engine::Model>("res/models/Earth.glb"), m_MoonSurfaceShader, "Earth");

		m_Player = m_EntityManager->CreateEntity(
			std::make_shared<Engine::Model>("res/models/Player.glb"), m_MoonSurfaceShader, "Player");

		m_MoonSurface->Scale(glm::vec3(10.0f));
		m_Earth->Scale(glm::vec3(3.0f));
		m_Earth->Rotate(glm::vec3(160.0f, 20.0f, 30.0f));
		m_Earth->Translate(glm::vec3(-3680.0f, 1500.0f, 0.0f));
		m_Player->Translate(glm::vec3(0.0f, 70.0f, 0.0f));
	}
}