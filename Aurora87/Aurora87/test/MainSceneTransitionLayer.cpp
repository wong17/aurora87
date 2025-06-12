#include "MainSceneTransitionLayer.h"

namespace Test
{
	MainSceneTransitionLayer::MainSceneTransitionLayer() : 
		TransitionLayer([](std::atomic<bool>&) { return std::make_shared<Test::MainSceneTestLayer>(); })
	{ }

	void MainSceneTransitionLayer::OnAttach()
	{
		TransitionLayer::OnAttach();

		// Shader Manager
		auto& shaderManager = Engine::ShaderManager::Get();
		m_TextShader = shaderManager.Load(
			"res/shaders/text_rendering/text-rendering-vertex-shader.glsl",
			"res/shaders/text_rendering/text-rendering-fragment-shader.glsl");

		auto& app = Engine::Application::Get();
		app.GetWindow().SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

		app.GetRenderSettings().SetAlphaBlending(true);
	}

	void MainSceneTransitionLayer::OnDetach()
	{
		TransitionLayer::OnDetach();
	}

	void MainSceneTransitionLayer::OnUpdate(float deltaTime)
	{
		TransitionLayer::OnUpdate(deltaTime);

		if (m_TextShader && m_TextRenderer)
		{
			m_TextShader->Bind();
			m_TextRenderer->RenderText(*m_TextShader, "Loading...", 10.0f, 10.0f, 1.0f, { 1.0f, 1.0f, 1.0f });

			m_HasRenderedAtLeastOnce = true;
		}

		if (m_HasRenderedAtLeastOnce)
			m_ElapsedTime += deltaTime;

		float effectiveMinTime = m_HasRenderedAtLeastOnce ? m_TransitionDelay : m_MinimumRenderTime;
		if (m_HasRenderedAtLeastOnce && m_ElapsedTime >= effectiveMinTime)
		{
			SetTransitionReady(true);
		}
	}
}