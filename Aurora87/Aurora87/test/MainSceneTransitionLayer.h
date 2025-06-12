#pragma once

#include "MainSceneTestLayer.h"

#include "Engine/Core/Application.h"
#include "Engine/Core/TransitionLayer.h"
#include "Engine/Core/RenderSettings.h"

#include "Engine/OpenGL/Skybox.h"
#include "Engine/OpenGL/TextRenderer.h"
#include "Engine/OpenGL/Shader.h"
#include "Engine/OpenGL/ShaderManager.h"
#include "Engine/OpenGL/ShaderDataTypes.h"
#include "Engine/OpenGL/OrthographicCamera.h"

#include "Engine/Audio/AudioManager.h"
#include "Engine/Audio/AudioSource.h"

#include <iostream>

namespace Test
{
	class MainSceneTransitionLayer : public Engine::TransitionLayer
	{
	public:
		MainSceneTransitionLayer();
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(float deltaTime) override;

	private:
		// Text Renderer
		std::shared_ptr<Engine::TextRenderer> m_TextRenderer = nullptr;
		std::shared_ptr<Engine::OrthographicCamera> m_OrthographicCamera = nullptr;
		std::shared_ptr<Engine::Shader> m_TextShader = nullptr;

		// Managers
		std::shared_ptr<Engine::AudioManager> m_AudioManager = nullptr;
		std::shared_ptr<Engine::AudioSource> m_CurrentAudioSource = nullptr;

		float m_ElapsedTime = 0.0f;
		bool m_HasRenderedAtLeastOnce = false;
		float m_TransitionDelay = 0.3f;
		float m_MinimumRenderTime = 0.03f;
	};
}