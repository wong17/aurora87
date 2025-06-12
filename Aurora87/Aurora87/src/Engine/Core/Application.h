#pragma once

#include "Window.h"
#include "Layer.h"
#include "LayerStack.h"
#include "RenderSettings.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/WindowEvent.h"
#include "Engine/ImGui/ImGuiLayer.h"
#include "Engine/OpenGL/Camera.h"
#include "Engine/OpenGL/OrthographicCamera.h"
#include "Engine/OpenGL/PerspectiveCamera.h"
#include "Engine/Audio/AudioEngine.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <iomanip>
#include <functional>

namespace Engine
{
	class Application
	{
	public:
		Application(const std::string& name = "Aurora 87", uint32_t width = 1920, uint32_t height = 1080);
		~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(std::shared_ptr<Layer> layer);
		void PushOverlay(std::shared_ptr<Layer> overlay);
		void PopLayer(std::shared_ptr<Layer> layer);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
		inline Camera& GetCamera() { return *m_Camera; }
		inline RenderSettings& GetRenderSettings() { return *m_RenderSettings; }
		inline ImGuiLayer& GetImGuiLayer() { return *m_ImGuiLayer; }
		inline AudioEngine& GetAudioEngine() { return m_AudioEngine; }
		inline LayerStack& GetLayerStack() { return m_LayerStack; }

		inline float GetDeltaTime() const { return m_DeltaTime; }
		inline float GetLastFrame() const { return m_LastFrame; }

		inline void Close() { m_Running = false; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		void ProcessLayerOperations();
	private:
		std::unique_ptr<Window> m_Window;
		std::shared_ptr<ImGuiLayer> m_ImGuiLayer;
		std::unique_ptr<Camera> m_Camera;
		std::unique_ptr<RenderSettings> m_RenderSettings;
		AudioEngine m_AudioEngine;
		LayerStack m_LayerStack;

		bool m_Running = true;
		float m_LastFrameTime = 0.0f;
		float m_DeltaTime = 0.0f, m_LastFrame = 0.0f;

		bool m_IsRunning = false;
		std::vector<std::function<void()>> m_PendingOperations;

		static Application* s_Instance;
	};
}