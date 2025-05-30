#include "AIEntitiesTestLayer.h"
#include "AIEnviroment/GeometricsCollision/Geometrics.h"
void Test::AIEntitiesTestLayer::OnAttach()
{
	// Texture Manager
	auto texture1 = Engine::TextureManager::Get().Load("res/textures/container.jpg");
	auto texture2 = Engine::TextureManager::Get().Load("res/textures/container2.png");

	// Shader Manager
	auto shapesShader = Engine::ShaderManager::Get().Load(
		"res/shaders/test/basic-vertex-shader.glsl", "res/shaders/test/basic-fragment-shader.glsl");
	shapesShader->Bind();
	shapesShader->SetInt("u_Texture", 0);

	auto modelShader = Engine::ShaderManager::Get().Load(
		"res/shaders/test/model-vertex-shader.glsl", "res/shaders/test/model-fragment-shader.glsl");

	m_TextShader = std::make_unique<Engine::Shader>(
		"res/shaders/text_rendering/text-rendering-vertex-shader.glsl",
		"res/shaders/text_rendering/text-rendering-fragment-shader.glsl"
	);

	// Instancing
	const unsigned int numBatches = 5;
	glm::vec3 areaMin(-20.0f, -20.0f, -20.0f);
	glm::vec3 areaMax(20.0f, 20.0f, 20.0f);
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
		"res/shaders/skybox/skybox-vertex-shader.glsl", "res/shaders/skybox/skybox-fragment-shader.glsl");
	m_Skybox = std::make_unique<Engine::Skybox>(faces, m_SkyboxShader);

	// Uniform Buffer
	m_CameraUniformBuffer = std::make_shared<Engine::UniformBuffer>(Engine::UniformBuffer::GetCameraLayout(), 0);
	m_ModelUniformBuffer = std::make_shared<Engine::UniformBuffer>(Engine::UniformBuffer::GetModelLayout(), 1);
	m_CameraUniformBuffer->BindToShader("Camera", { *shapesShader, *modelShader });
	m_ModelUniformBuffer->BindToShader("Model", { *shapesShader, *modelShader });

	// Entity Manager
	m_EntityManager = std::make_unique<Engine::EntityManager>(*m_ModelUniformBuffer);
	m_TestCube = m_EntityManager->CreateEntity(Engine::PrimitiveFactory::CreateCube(texture1), shapesShader, "TestCube");
	m_TestPyramid = m_EntityManager->CreateEntity(Engine::PrimitiveFactory::CreatePyramid(texture2), shapesShader, "TestPyramid");
	m_TestPentagon = m_EntityManager->CreateEntity(Engine::PrimitiveFactory::CreatePentagon(texture1), shapesShader, "TestPentagon");
	m_TestHexagon = m_EntityManager->CreateEntity(Engine::PrimitiveFactory::CreateHexagon(texture2), shapesShader, "TestHexagon");
	m_TestIcosphere = m_EntityManager->CreateEntity(Engine::PrimitiveFactory::CreateIcosphere(texture1), shapesShader, "TestIcosphere");
	m_Model = m_EntityManager->CreateEntity(std::make_shared<Engine::Model>("res/models/ModelMenu/ModelMenu.obj"), modelShader, "Model");

	m_EntityManager->SetInstanceTransforms({
		{ m_TestCube,      batches[0] },
		{ m_TestPyramid,   batches[1] },
		{ m_TestPentagon,  batches[2] },
		{ m_TestHexagon,   batches[3] },
		{ m_TestIcosphere, batches[4] }
		});

	// Scene Renderer
	m_SceneRenderer = std::make_shared<Engine::SceneRenderer>(*m_CameraUniformBuffer);
	// Transform System
	m_TransformSystem = std::make_shared<Engine::TransformSystem>(*m_EntityManager);

	auto& app = Engine::Application::Get();

	// Orthographic Camera
	m_GameCamera = std::make_shared<Engine::CameraGameBeta>(
		app.GetWindow().GetWidth(), app.GetWindow().GetHeight(), glm::vec3(0.0f, 0.0f, 0.f));

	// Text Renderer
	m_TextRenderer = std::make_unique<Engine::TextRenderer>("res/fonts/BlackOpsOne-Regular.ttf", 32);
	m_TextShader->Bind();
	m_TextShader->SetMat4("u_ProjectionMatrix", m_GameCamera->Projection);

	// Desactivar el cursor para el movimiento de la cámara
	app.GetWindow().SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Configuración de OpenGL
	app.GetRenderSettings().SetDepthTest(true);
	app.GetRenderSettings().SetCullFace(true);
	app.GetRenderSettings().SetAlphaBlending(true);
	Player* player = new Player();
	DumbZombie* dumbZombie = new DumbZombie();
	m_Player = player;
	m_DumbZombie = dumbZombie;
	m_Player->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	GameEntityManager::Instance().RegisterEntity(m_Player);
	GameEntityManager::Instance().RegisterEntity(m_DumbZombie);
	AIEnviroment::LittleHealthEntity* k = new AIEnviroment::LittleHealthEntity();
	AIEnviroment::NormalHealthEntity* k1 = new AIEnviroment::NormalHealthEntity();
	AIEnviroment::BigHealthEntity* k2 = new AIEnviroment::BigHealthEntity();
	GameEntityManager::Instance().RegisterEntity(k);
	GameEntityManager::Instance().RegisterEntity(k1);
	GameEntityManager::Instance().RegisterEntity(k2);

	m_Player->SetTypesHealth(k);
	m_Player->SetTypesHealth(k1);
	m_Player->SetTypesHealth(k2);
	for (auto const& h : m_Player->GetTypesHealth())
	{
		std::cout << h->GetCantHealth() << std::endl;
	}
}

void Test::AIEntitiesTestLayer::OnDetach()
{

}

void Test::AIEntitiesTestLayer::OnUpdate(float deltaTime)
{
	auto& app = Engine::Application::Get();

	glm::mat4 view = m_GameCamera->View;
	glm::mat4 projection = m_GameCamera->Projection;
	m_GameCamera->Inputs(app.GetWindow().GetNativeWindow(), m_Player->GetID());
	m_GameCamera->Matrix(45.0f, 0.1f, 100.0f);
	// Actualizar la matriz de vista y proyección de todas las entidades
	m_SceneRenderer->SetViewProjection(view, projection);

	// Calcular la posición de las entidades
	m_Angle += m_AngularSpeed * deltaTime * 0.4f;
	if (m_Angle > glm::two_pi<float>())
		m_Angle -= glm::two_pi<float>();
	glm::vec3 position{ std::sin(m_Angle) * m_Radius , 0.0f, std::cos(m_Angle) * m_Radius };
	m_Player->Update();
	m_Player->SetDeltaTime(deltaTime);
	m_DumbZombie->Update();
	m_DumbZombie->SetDeltaTime(deltaTime);
	//m_Player->SetPosition(m_GameCamera->Position);

	m_TransformSystem->TranslateEntity( m_TestCube , m_DumbZombie->GetPosition());
	m_TransformSystem->TranslateEntity( m_TestIcosphere , m_Player->GetPosition());
	m_SceneRenderer->RenderAll(*m_EntityManager, true);

	// Dibujar el Skybox
	m_Skybox->Draw(view, projection);
	Collision::AABB aabb = Collision::AABB(m_Player->GetPosition(), glm::vec3(0.5f, 0.5f, 0.5f));
	Collision::AABBPLANES aabbPlanes(aabb);
	Collision::GeometricsBox geometricsBox;
	Collision::AABB ab = geometricsBox.Calculate_AABB(m_TestCube->GetMesh());
	//std::cout << "AABB de la caja de prueba: " << std::endl;
	//std::cout << ab.m_min.x << ", " << ab.m_min.y << ", " << ab.m_min.z << std::endl; 
	// Dibujar el texto
	m_TextShader->Bind();
	m_TextRenderer->RenderText(*m_TextShader, "Black Ops One Font", 10.0f, 10.0f, 1.0f, { 1.0f,1.0f,1.0f });

}

void Test::AIEntitiesTestLayer::OnImGuiRender()
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

void Test::AIEntitiesTestLayer::OnEvent(Engine::Event& event)
{
	Engine::Application::Get().GetCamera().OnEvent(event);
	if (event.GetEventType() == Engine::EventType::MouseScrolled)
	{
		auto& mouseScrolledEvent = static_cast<Engine::MouseScrolledEvent&>(event);
		OnMouseScroll(mouseScrolledEvent);
	}
}

void Test::AIEntitiesTestLayer::OnMouseScroll(Engine::MouseScrolledEvent& event)
{
	auto* CameraGameBeta = static_cast<Engine::CameraGameBeta*>(&Engine::Application::Get().GetCamera());

	glm::mat4 projection = CameraGameBeta->GetProjectionMatrix();

	// Actualizar la matriz de proyección en el shader
	m_CameraUniformBuffer->Bind();
	m_CameraUniformBuffer->SetData("u_ProjectionMatrix", glm::value_ptr(projection));

	m_Skybox->SetProjectionMatrix(projection);

}
