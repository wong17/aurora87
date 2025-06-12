#include "Window.h"

namespace Engine
{
	static bool s_GLFWInitialized = false;

	// Error callback

	static void GLFWErrorCallback(int error, const char* description)
	{
		std::cerr << "[GLFW Error] (" << error << "): " << description << std::endl;
	}

	// Window

	Window::Window(const WindowProperties& props) : m_GLFWwindow(nullptr), m_WindowData({ props })
	{
		m_WindowData.Properties = props;
		// Configura callback de error por defecto
		glfwSetErrorCallback(GLFWErrorCallback);
		// Inicializa la ventana
		if (!Init()) 
		{
			std::cerr << "Error al inicializar la ventana GLFW." << std::endl;
			m_GLFWwindow = nullptr;
		}
	}

	Window::~Window()
	{
		if (m_GLFWwindow)
			Shutdown();

		//std::cout << "Window destroyed." << std::endl;

		glfwTerminate();
		s_GLFWInitialized = false;

		//std::cout << "GLFW terminated." << std::endl;
	}

	bool Window::Init()
	{
		if (!s_GLFWInitialized)
		{
			// Inicializa GLFW
			int success = glfwInit();
			if (!success)
			{
				std::cerr << "Error al inicializar GLFW." << std::endl;
				return false;
			}
			s_GLFWInitialized = true;
		}

		// Establece hints para la versión de OpenGL y el perfil a utilizar
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE); // COMENTAR ESTO EN RELEASE, ES SOLO PARA DEBUG
#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		// Crea la ventana
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		if (m_WindowData.Properties.BorderlessFullscreen)
		{
			m_WindowData.Properties.Width = mode->width;
			m_WindowData.Properties.Height = mode->height;

			glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		}
		else
		{
			glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
			glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
		}

		m_GLFWwindow = glfwCreateWindow(m_WindowData.Properties.Width, m_WindowData.Properties.Height, 
			m_WindowData.Properties.Title.c_str(), nullptr, nullptr);
		if (!m_GLFWwindow)
		{
			glfwTerminate();
			return false;
		}
		// Establece el contexto de OpenGL
		glfwMakeContextCurrent(m_GLFWwindow);
		// Inicializa GLAD
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cerr << "Error al inicializar GLAD." << std::endl;
			return false;
		}
		// Habilita el modo de depuración de OpenGL
		Engine::OpenGLDebug::Enable();

		// Esto permite que los callbacks de GLFW puedan acceder a los datos y métodos de la instancia de la clase Window
		glfwSetWindowUserPointer(m_GLFWwindow, &m_WindowData);
		// Establece callbacks para GLFW
		glfwSetKeyCallback(m_GLFWwindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						KeyPressedEvent event(key, 0);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(key);
						data.EventCallback(event);
						break;
					}

					case GLFW_REPEAT:
					{
						KeyPressedEvent event(key, 1);
						data.EventCallback(event);
						break;
					}
				}
			});

		glfwSetFramebufferSizeCallback(m_GLFWwindow, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				glViewport(0, 0, width, height);
			});

		glfwSetWindowSizeCallback(m_GLFWwindow, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Properties.Width = width;
				data.Properties.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(m_GLFWwindow, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				WindowCloseEvent event;
				data.EventCallback(event);
			});

		glfwSetCharCallback(m_GLFWwindow, [](GLFWwindow* window, unsigned int codepoint)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				KeyTypedEvent event(codepoint);
				data.EventCallback(event);
			});

		glfwSetMouseButtonCallback(m_GLFWwindow, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent event(button);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent event(button);
						data.EventCallback(event);
						break;
					}
				}
			});

		glfwSetScrollCallback(m_GLFWwindow, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xoffset, (float)yoffset);
				data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_GLFWwindow, [](GLFWwindow* window, double xpos, double ypos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xpos, (float)ypos);
				data.EventCallback(event);
			});

		return true;
	}

	void Window::OnUpdate()
	{
		// Intercambia buffers y procesa eventos
		glfwSwapBuffers(m_GLFWwindow);
		glfwPollEvents();
	}

	void Window::Shutdown()
	{
		glfwDestroyWindow(m_GLFWwindow);
		m_GLFWwindow = nullptr;
	}

	void Window::SetInputMode(int mode, int value)
	{
		if (m_GLFWwindow)
			glfwSetInputMode(m_GLFWwindow, mode, value);
	}

	int Window::GetInputMode(int mode) const
	{
		if (m_GLFWwindow)
			return glfwGetInputMode(m_GLFWwindow, mode);
		return -1;
	}

	void Window::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_WindowData.VSync = enabled;
	}
}