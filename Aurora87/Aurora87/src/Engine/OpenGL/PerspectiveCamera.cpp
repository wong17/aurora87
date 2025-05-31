#include "PerspectiveCamera.h"

namespace Engine
{
	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
		RecalculateViewMatrix();
	}

	void PerspectiveCamera::SetProjection(float fov, float aspectRatio, float nearClip, float farClip)
	{
		m_FOV = fov;
		m_AspectRatio = aspectRatio;
		m_NearClip = nearClip;
		m_FarClip = farClip;
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::SetRotation(const glm::vec3& rotation)
	{
		m_Rotation = rotation;
		m_Yaw = m_Rotation.y;
		// Limitar el rango de pitch para evitar problemas de gimbal lock o bloqueo Cardan
		m_Pitch = std::clamp(rotation.x, -89.0f, 89.0f); // Limitar el rango de pitch entre -89 y 89 grados
		m_Roll = m_Rotation.z;
		RecalculateViewMatrix();
	}

	void PerspectiveCamera::SetYaw(float yaw)
	{
		m_Yaw = std::clamp(yaw, m_MinYaw, m_MaxYaw);
		RecalculateViewMatrix();
	}

	void PerspectiveCamera::SetPitch(float pitch)
	{
		m_Pitch = std::clamp(pitch, m_MinPitch, m_MaxPitch);
		RecalculateViewMatrix();
	}

	void PerspectiveCamera::SetRoll(float roll)
	{
		m_Roll = roll;
		RecalculateViewMatrix();
	}

	void PerspectiveCamera::SetFOV(float fov)
	{
		SetProjection(fov, m_AspectRatio, m_NearClip, m_FarClip);
	}

	void PerspectiveCamera::AdjustFOV(float deltaFOV, float minFOV, float maxFOV)
	{
		m_FOV -= deltaFOV;
		m_FOV = std::clamp(m_FOV, minFOV, maxFOV); // Limitar el FOV entre minFOV y maxFOV
		SetProjection(m_FOV, m_AspectRatio, m_NearClip, m_FarClip);
	}

	void PerspectiveCamera::SetAspectRatio(float aspectRatio)
	{
		SetProjection(m_FOV, aspectRatio, m_NearClip, m_FarClip);
	}

	void PerspectiveCamera::SetNearClip(float nearClip)
	{
		SetProjection(m_FOV, m_AspectRatio, nearClip, m_FarClip);
	}

	void PerspectiveCamera::SetFarClip(float farClip)
	{
		SetProjection(m_FOV, m_AspectRatio, m_NearClip, farClip);
	}

	void PerspectiveCamera::OnEvent(Event& event)
	{
		if (event.GetEventType() == Engine::EventType::KeyPressed)
		{
			auto& keyEvent = static_cast<Engine::KeyPressedEvent&>(event);

			// Si se presiona la tecla ESC, alterna la captura del mouse
			if (keyEvent.GetKeyCode() == GLFW_KEY_ESCAPE)
			{
				m_MouseCaptured = !m_MouseCaptured;
				auto& window = Engine::Application::Get().GetWindow();
				if (m_MouseCaptured)
				{
					m_FirstMouse = true; // Reiniciar la posición inicial del mouse
					window.SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				}
				else
				{
					window.SetInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				}
				return;
			}
		}

		if (event.GetEventType() == Engine::EventType::KeyPressed && m_KeyboardEnabled)
		{
			auto& keyEvent = static_cast<Engine::KeyPressedEvent&>(event);
			float deltaTime = Engine::Application::Get().GetDeltaTime();
			switch (keyEvent.GetKeyCode())
			{
				case 'W': ProcessKeyboard(CameraMovement::FORWARD, deltaTime); break;
				case 'S': ProcessKeyboard(CameraMovement::BACKWARD, deltaTime); break;
				case 'A': ProcessKeyboard(CameraMovement::LEFT, deltaTime); break;
				case 'D': ProcessKeyboard(CameraMovement::RIGHT, deltaTime); break;
				default: break;
			}
		}
		else if (event.GetEventType() == Engine::EventType::MouseMoved)
		{
			// Solo procesamos el movimiento del mouse si el cursor está capturado
			if (!m_MouseEnabled || !m_MouseCaptured)
				return;

			auto& mouseEvent = static_cast<Engine::MouseMovedEvent&>(event);
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
			float yOffset = m_LastY - yPos; // Invertir el eje Y
			m_LastX = xPos;
			m_LastY = yPos;

			ProcessMouseMovement(xOffset, yOffset);
		}
		else if (event.GetEventType() == Engine::EventType::MouseScrolled)
		{
			auto& mouseScrolledEvent = static_cast<Engine::MouseScrolledEvent&>(event);
			// Ajustar el FOV de la cámara al hacer scroll
			AdjustFOV(mouseScrolledEvent.GetYOffset());
		}
	}

	void PerspectiveCamera::ProcessKeyboard(CameraMovement movement, float deltaTime)
	{
		if (!m_KeyboardEnabled)
			return;

		float velocity = m_MovementSpeed * deltaTime;
		glm::vec3 position = m_Position;

		switch (movement)
		{
			case CameraMovement::FORWARD:	position += velocity * m_Front; break;
			case CameraMovement::BACKWARD:	position -= velocity * m_Front; break;
			case CameraMovement::LEFT:		position -= velocity * m_Right; break;
			case CameraMovement::RIGHT:		position += velocity * m_Right; break;
			default: std::cout << "PerspectiveCamera::ProcessKeyboard: Invalid Camera Movement" << std::endl; break;
		}
		// Actualiza la posición de la cámara y recalcula la matriz de vista
		SetPosition(position);
	}

	void PerspectiveCamera::ProcessMouseMovement(float xOffset, float yOffset)
	{
		if (!m_MouseEnabled)
			return;

		// Esto es para invertir el movimiento del mouse
		xOffset *= m_MouseSensitivity;
		yOffset *= m_MouseSensitivity;
		// Actualiza los ángulos de yaw y pitch
		m_Yaw += xOffset;
		m_Pitch += yOffset;
		
		if (m_YawLimitsEnabled) 
		{
			m_Yaw = std::clamp(m_Yaw, m_MinYaw, m_MaxYaw);
		}
		m_Pitch = std::clamp(m_Pitch, m_MinPitch, m_MaxPitch);
		RecalculateViewMatrix();
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		// Calcula el vector de dirección a partir de yaw y pitch
		glm::vec3 front{};
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Front = glm::normalize(front);

		// Calcula el vector de dirección a la derecha y arriba de la cámara
		m_Right = glm::normalize(glm::cross(m_Front, glm::vec3(0.0f, 1.0f, 0.0f)));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));
		
		// Genera la matriz de vista usando glm::lookAt
		glm::mat4 view = glm::lookAt(m_Position, m_Position + m_Front, m_Up);

		// Aplica la rotación (roll) a la matriz de vista
		if (m_Roll != 0.0f)
		{
			glm::mat4 rollMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(m_Roll), m_Front);
			view = rollMatrix * view;
		}

		// Actualiza la matriz de vista y la matriz de proyección
		m_ViewMatrix = view;
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}