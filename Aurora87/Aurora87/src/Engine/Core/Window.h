#pragma once

#include "Engine/Events/Event.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Events/WindowEvent.h"
#include "Engine/Core/OpenGLDebug.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <functional>

namespace Engine
{
	struct WindowProperties
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		bool BorderlessFullscreen = false;

		WindowProperties(
			const std::string& title, 
			uint32_t width, 
			uint32_t height, 
			bool borderlessFullscreen = false)
			: Title(title), Width(width), Height(height), BorderlessFullscreen(borderlessFullscreen) { }
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window(const WindowProperties& props);
		~Window();

		void OnUpdate();
		void Shutdown();

		void SetInputMode(int mode, int value);
		int GetInputMode(int mode) const;

		inline uint32_t GetWidth() const { return m_WindowData.Properties.Width; }
		inline uint32_t GetHeight() const { return m_WindowData.Properties.Height; }
		inline GLFWwindow* GetNativeWindow() const { return m_GLFWwindow; }
		void SetVSync(bool enabled = 1);
		inline bool IsVSync() const { return m_WindowData.VSync; }

		inline void SetEventCallback(const EventCallbackFn& callback) { m_WindowData.EventCallback = callback; }

	private:
		GLFWwindow* m_GLFWwindow;

		struct WindowData
		{
			WindowProperties Properties;
			EventCallbackFn EventCallback;
			bool VSync;
		};
		WindowData m_WindowData;

		bool Init();
	};
}