#include "CameraGame.h"
#include <unordered_set>


namespace Engine {



	CameraGame::CameraGame(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
		RecalculateViewMatrix();
	}
	void CameraGame::SetProjection(float fov, float aspectRatio, float nearClip, float farClip)
	{
		m_FOV = fov;
		m_AspectRatio = aspectRatio;
		m_NearClip = nearClip;
		m_FarClip = farClip;
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void CameraGame::SetRotation(const glm::vec3& rotation)
	{
		m_Rotation = rotation;
		m_Yaw = m_Rotation.y;
		m_Pitch = std::clamp(rotation.x, -89.0f, 89.0f);
		m_Roll = m_Rotation.z;
		RecalculateViewMatrix();
	}

	void CameraGame::SetYaw(float yaw)
	{
		m_Yaw = yaw;
		RecalculateViewMatrix();
	}

	void CameraGame::SetPitch(float pitch)
	{
		m_Pitch = std::clamp(pitch, -89.0f, 89.0f);
		RecalculateViewMatrix();
	}

	void CameraGame::SetRoll(float roll)
	{
		m_Roll = roll;
		RecalculateViewMatrix();
	}

	void CameraGame::SetFOV(float fov)
	{
		SetProjection(fov, m_AspectRatio, m_NearClip, m_FarClip);
	}

	void CameraGame::AdjustFOV(float deltaFOV, float minFOV, float maxFOV)
	{
		m_FOV -= deltaFOV;
		m_FOV = std::clamp(m_FOV, minFOV, maxFOV);
		SetProjection(m_FOV, m_AspectRatio, m_NearClip, m_FarClip);
	}

	void CameraGame::SetAspectRatio(float aspectRatio)
	{
		SetProjection(m_FOV, aspectRatio, m_NearClip, m_FarClip);
	}

	void CameraGame::SetNearClip(float nearClip)
	{
		SetProjection(m_FOV, m_AspectRatio, nearClip, m_FarClip);
	}

	void CameraGame::SetFarClip(float farClip)
	{
		SetProjection(m_FOV, m_AspectRatio, m_NearClip, farClip);
	}
	void CameraGame::Update(float deltaTime)
	{
		if (!m_MouseCaptured)
			return;

		auto& window = Engine::Application::Get().GetWindow();
		GLFWwindow* nativeWindow = static_cast<GLFWwindow*>(window.GetNativeWindow());

		if (glfwGetKey(nativeWindow, GLFW_KEY_W) == GLFW_PRESS)
			ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
		if (glfwGetKey(nativeWindow, GLFW_KEY_S) == GLFW_PRESS)
			ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
		if (glfwGetKey(nativeWindow, GLFW_KEY_A) == GLFW_PRESS)
			ProcessKeyboard(CameraMovement::LEFT, deltaTime);
		if (glfwGetKey(nativeWindow, GLFW_KEY_D) == GLFW_PRESS)
			ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
	}

	void CameraGame::OnEvent(Event& event)
	{
		float deltaTime = Engine::Application::Get().GetDeltaTime();

		switch (event.GetEventType())
		{
		case EventType::KeyPressed:
		{
			auto& keyEvent = static_cast<KeyPressedEvent&>(event);
			int key = keyEvent.GetKeyCode();

			if (key == GLFW_KEY_ESCAPE)
			{
				m_MouseCaptured = !m_MouseCaptured;
				auto& window = Engine::Application::Get().GetWindow();

				if (m_MouseCaptured)
				{
					m_FirstMouse = true;
					window.SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				}
				else
				{
				window.SetInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				}
				return;
			}

			if (m_MouseCaptured)
			{
				m_KeysPressed.insert(key); // Agregar tecla presionada
			}

			break;
		}
		case EventType::KeyReleased:
		{
			auto& keyEvent = static_cast<KeyReleasedEvent&>(event);
			int key = keyEvent.GetKeyCode();

			if (m_MouseCaptured)
			{
				m_KeysPressed.erase(key); // Eliminar tecla soltada
			}

			break;
		}
		case EventType::MouseMoved:
		{
			if (!m_MouseCaptured)
				return;

			auto& mouseEvent = static_cast<MouseMovedEvent&>(event);
			float xPos = mouseEvent.GetX();
			float yPos = mouseEvent.GetY();

			if (m_FirstMouse)
			{
				m_LastX = xPos;
				m_LastY = yPos;
				m_FirstMouse = false;
				return;
		}

			float xOffset = xPos - m_LastX;
			float yOffset = m_LastY - yPos;

			m_LastX = xPos;
			m_LastY = yPos;

			ProcessMouseMovement(xOffset, yOffset);
			break;
		}
		case EventType::MouseScrolled:
		{
			auto& scrollEvent = static_cast<MouseScrolledEvent&>(event);
			AdjustFOV(scrollEvent.GetYOffset());
			break;
		}
		}
		
	}

	void CameraGame::ProcessKeyboard(CameraMovement movement, float deltaTime)
	{
		float velocity = m_MovementSpeed * deltaTime;

		switch (movement)
		{
		case CameraMovement::FORWARD: m_Position += m_Front * velocity; break;
		case CameraMovement::BACKWARD: m_Position -= m_Front * velocity; break;
		case CameraMovement::LEFT: m_Position -= m_Right * velocity; break;
		case CameraMovement::RIGHT: m_Position += m_Right * velocity; break;
		}

		RecalculateViewMatrix();
	}

	void CameraGame::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch)
	{
		xOffset *= m_MouseSensitivity;
		yOffset *= m_MouseSensitivity;
		m_Yaw += xOffset;
		m_Pitch += yOffset;
		if (constrainPitch)
		{
			m_Pitch = std::clamp(m_Pitch, -89.0f, 89.0f);
		}
		RecalculateViewMatrix();
	}

	void CameraGame::HandleEvent(const EventData& event)
	{
	}

	void CameraGame::RecalculateViewMatrix()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Front = glm::normalize(front);

		m_Right = glm::normalize(glm::cross(m_Front, glm::vec3(0.0f, 1.0f, 0.0f)));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));

		std::cout << "Yaw: " << m_Yaw << ", Pitch: " << m_Pitch << std::endl;
		std::cout << "Front: (" << m_Front.x << ", " << m_Front.y << ", " << m_Front.z << ")\n";
		std::cout << "Right: (" << m_Right.x << ", " << m_Right.y << ", " << m_Right.z << ")\n";

		glm::mat4 view = glm::lookAt(m_Position, m_Position + m_Front, m_Up);

		if (m_Roll != 0.0f)
		{
			glm::mat4 rollMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(m_Roll), m_Front);
			view = rollMatrix * view;
		}

		m_ViewMatrix = view;
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}