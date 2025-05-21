#include "SimpleGammaCorrectionTestLayer.h"

namespace Test
{
	void SimpleGammaCorrectionTestLayer::OnAttach()
	{
		// Texture Manager
		Engine::TextureSpecification spec;
		spec.SRGB = true;
		auto texture1 = Engine::TextureManager::Get().Load("res/textures/container.jpg", spec);
		auto texture2 = Engine::TextureManager::Get().Load("res/textures/container2.png", spec);
		auto texture3 = Engine::TextureManager::Get().Load("res/textures/wood.png", spec);

		// Shader Manager
		m_ShapesShader = Engine::ShaderManager::Get().Load(
			"res/shaders/test/texture-instanced-vertex-shader.glsl", "res/shaders/test/gamma-correction-fragment-shader.glsl");
		m_ShapesShader->Bind();
		m_ShapesShader->SetInt("u_Texture", 0);

		m_PlaneShader = Engine::ShaderManager::Get().Load(
			"res/shaders/test/texture-vertex-shader.glsl", "res/shaders/test/gamma-correction-fragment-shader.glsl");
		m_PlaneShader->Bind();
		m_PlaneShader->SetInt("u_Texture", 0);

		m_TextShader = std::make_unique<Engine::Shader>(
			"res/shaders/text_rendering/text-rendering-vertex-shader.glsl",
			"res/shaders/text_rendering/text-rendering-fragment-shader.glsl"
		);

		// Instancing
		const unsigned int numBatches = 2;
		glm::vec3 areaMin(-50.0f, -50.0f, -50.0f);
		glm::vec3 areaMax(50.0f, 50.0f, 50.0f);
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
		m_CameraUniformBuffer = std::make_shared<Engine::UniformBuffer>(Engine::UniformBuffer::GetCameraLayout(), 0);
		m_ModelUniformBuffer = std::make_shared<Engine::UniformBuffer>(Engine::UniformBuffer::GetModelLayout(), 1);
		m_CameraUniformBuffer->BindToShader("Camera", { *m_ShapesShader, *m_PlaneShader });
		m_ModelUniformBuffer->BindToShader("Model", { *m_ShapesShader, *m_PlaneShader });

		// Entity Manager
		m_EntityManager = std::make_unique<Engine::EntityManager>(*m_ModelUniformBuffer);
		m_TestCube = m_EntityManager->CreateEntity(Engine::PrimitiveFactory::CreateCube(texture1), m_ShapesShader, "TestCube");
		m_TestPentagon = m_EntityManager->CreateEntity(Engine::PrimitiveFactory::CreatePentagon(texture2), m_ShapesShader, "TestPentagon");
		m_TestPlane = m_EntityManager->CreateEntity(Engine::PrimitiveFactory::CreatePlane(texture3), m_PlaneShader, "TestPlane");

		m_EntityManager->SetInstanceTransforms({
			{ m_TestCube, batches[0] },
			{ m_TestPentagon, batches[1] }
			});

		m_SceneRenderer = std::make_shared<Engine::SceneRenderer>(*m_CameraUniformBuffer);
		// Transform System
		m_TransformSystem = std::make_shared<Engine::TransformSystem>(*m_EntityManager);
		m_TransformSystem->TranslateEntity(m_TestPlane, { 0.0f, -0.5f, 0.0f });

		auto& app = Engine::Application::Get();

		// Orthographic Camera
		m_OrthographicCamera = std::make_shared<Engine::OrthographicCamera>(
			0.0f, static_cast<float>(app.GetWindow().GetWidth()),
			0.0f, static_cast<float>(app.GetWindow().GetHeight()));

		// Text Renderer
		m_TextRenderer = std::make_unique<Engine::TextRenderer>("res/fonts/BlackOpsOne-Regular.ttf", 32);
		m_TextShader->Bind();
		m_TextShader->SetMat4("u_ProjectionMatrix", m_OrthographicCamera->GetProjectionMatrix());

		// Desactivar el cursor para el movimiento de la cámara
		app.GetWindow().SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		app.GetRenderSettings().SetDepthTest(true);
		app.GetRenderSettings().SetCullFace(true);
		app.GetRenderSettings().SetAlphaBlending(true);
	}

	void SimpleGammaCorrectionTestLayer::OnDetach()
	{
	}

	void SimpleGammaCorrectionTestLayer::OnUpdate(float deltaTime)
	{
		auto& app = Engine::Application::Get();
		auto* camera = static_cast<Engine::PerspectiveCamera*>(&app.GetCamera());

		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection = camera->GetProjectionMatrix();

		// Actualizar la matriz de vista y proyección de todas las entidades
		m_SceneRenderer->SetViewProjection(view, projection);

		// Renderizar todas las entidades
		m_SceneRenderer->RenderAll(*m_EntityManager, true);

		m_ShapesShader->Bind();
		m_ShapesShader->SetFloat3("u_LightPosition", m_LightPosition);
		m_ShapesShader->SetFloat3("u_ViewPosition", camera->GetPosition());

		// Dibujar el Skybox
		m_Skybox->Draw(view, projection);

		// Dibujar el texto
		m_TextShader->Bind();
		m_TextRenderer->RenderText(*m_TextShader, "Simple Gamma Correction", 10.0f, 10.0f, 1.0f, { 1.0f,1.0f,1.0f });
	}

	void SimpleGammaCorrectionTestLayer::OnImGuiRender()
	{
		ImGui::Begin("Texture Test Layer");
		if (ImGui::Button("Toggle Wireframe Mode"))
		{
			m_WireframeMode = !m_WireframeMode;
			if (m_WireframeMode)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
		}
		ImGui::End();
	}

	void SimpleGammaCorrectionTestLayer::OnEvent(Engine::Event& event)
	{
		Engine::Application::Get().GetCamera().OnEvent(event);
		if (event.GetEventType() == Engine::EventType::MouseScrolled)
		{
			auto& mouseScrolledEvent = static_cast<Engine::MouseScrolledEvent&>(event);
			OnMouseScroll(mouseScrolledEvent);
		}
	}

	void SimpleGammaCorrectionTestLayer::OnMouseScroll(Engine::MouseScrolledEvent& event)
	{
		auto* perspectiveCamera = static_cast<Engine::PerspectiveCamera*>(&Engine::Application::Get().GetCamera());

		glm::mat4 projection = perspectiveCamera->GetProjectionMatrix();

		// Actualizar la matriz de proyección en el shader
		m_CameraUniformBuffer->Bind();
		m_CameraUniformBuffer->SetData("u_ProjectionMatrix", glm::value_ptr(projection));

		m_Skybox->SetProjectionMatrix(projection);
	}
}