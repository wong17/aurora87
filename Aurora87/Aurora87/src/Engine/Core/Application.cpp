#include "Application.h"

namespace Engine
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name, uint32_t width, uint32_t height)
	{
		s_Instance = this;
		m_Window = std::make_unique<Window>(WindowProperties(name, width, height, true));
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
		
		m_RenderSettings = std::make_unique<RenderSettings>(*m_Window);

		if (m_AudioEngine.Initialize() != InitResult::OK)
		{
			std::cerr << "Application::Application: Error inicializando AudioEngine\n";
		}
		else
		{
			m_AudioEngine.SetListenerGain(1.0f);
			m_AudioEngine.SetListenerPosition(0.0f, 0.0f, 0.0f);
			m_AudioEngine.PrintDeviceInfo();
		}

		float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		m_Camera = std::make_unique<PerspectiveCamera>(45.0f, aspectRatio, 0.1f, 1000.0f);
		m_Camera->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));

		m_ImGuiLayer = std::make_shared<ImGuiLayer>();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		m_AudioEngine.Shutdown();
	}

	void Application::Run()
	{
		float fpsTimer = 0.0f;
		int frameCount = 0;

		m_IsRunning = true;

		while (m_Running)
		{
			float currentFrame = static_cast<float>(glfwGetTime());
			m_DeltaTime = currentFrame - m_LastFrame;
			m_LastFrame = currentFrame;

			// Acumular tiempo y contar frames
			fpsTimer += m_DeltaTime;
			frameCount++;

			// Si ha pasado 1 segundo, calculamos los FPS y actualizamos el título
			if (fpsTimer >= 1.0f)
			{
				float fps = frameCount / fpsTimer;
				std::stringstream ss;
				ss << "Aurora 87 - FPS: " << std::fixed << std::setprecision(2) << fps;
				glfwSetWindowTitle(m_Window->GetNativeWindow(), ss.str().c_str());
				// Reiniciamos el contador y timer
				frameCount = 0;
				fpsTimer = 0.0f;
			}

			m_RenderSettings->ResetFrame();

			for (auto& layer : m_LayerStack)
				layer->OnUpdate(m_DeltaTime);

			ProcessLayerOperations();

			m_ImGuiLayer->Begin();

			m_RenderSettings->OnImGuiRender();
			for (auto& layer : m_LayerStack)
				layer->OnImGuiRender();
			
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}

		m_IsRunning = false;
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&Application::OnWindowResize, this, std::placeholders::_1));

		for (auto iterator = m_LayerStack.end(); iterator != m_LayerStack.begin();)
		{
			(*--iterator)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(std::shared_ptr<Layer> layer)
	{
		if (m_IsRunning) 
		{
			m_PendingOperations.push_back([this, layer]() { m_LayerStack.PushLayer(layer); });
		}
		else 
		{
			m_LayerStack.PushLayer(layer);
		}
	}

	void Application::PushOverlay(std::shared_ptr<Layer> overlay)
	{
		if (m_IsRunning)
		{
			m_PendingOperations.push_back([this, overlay]() { m_LayerStack.PushOverlay(overlay); });
		}
		else
		{
			m_LayerStack.PushOverlay(overlay);
		}
	}

	void Application::PopLayer(std::shared_ptr<Layer> layer)
	{
		if (m_IsRunning) 
		{
			m_PendingOperations.push_back([this, layer]() { m_LayerStack.PopLayer(layer.get()); });
		}
		else 
		{
			m_LayerStack.PopLayer(layer.get());
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		// Si la ventana es minimizada, no hace falta actualizar la camara
		if (e.GetWidth() == 0 || e.GetHeight() == 0) return false;

		if (m_Camera)
		{
			auto perspectiveCamera = static_cast<PerspectiveCamera*>(m_Camera.get());
			m_Camera->SetProjection(perspectiveCamera->GetFOV(), perspectiveCamera->GetAspectRatio(), 
				perspectiveCamera->GetNearClip(), perspectiveCamera->GetFarClip());
		}

		return false;
	}

	void Application::ProcessLayerOperations()
	{
		for (auto& op : m_PendingOperations) 
		{
			op();
		}
		m_PendingOperations.clear();
	}
}