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
		// Limit the pitch range to avoid gimbal lock problems.
		m_Pitch = std::clamp(rotation.x, -89.0f, 89.0f); // Limit pitch range between -89 and 89 degrees
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
		m_FOV = std::clamp(m_FOV, minFOV, maxFOV); // Limiting FOV between minFOV and maxFOV
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

			// If ESC key is pressed, toggles mouse capture
			if (keyEvent.GetKeyCode() == GLFW_KEY_ESCAPE)
			{
				m_MouseCaptured = !m_MouseCaptured;
				auto& window = Engine::Application::Get().GetWindow();
				if (m_MouseCaptured)
				{
					m_FirstMouse = true; // Reset the initial mouse position
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
			// We only process the mouse movement if the cursor is captured.
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
			float yOffset = m_LastY - yPos; // Invert Y-axis
			m_LastX = xPos;
			m_LastY = yPos;

			ProcessMouseMovement(xOffset, yOffset);
		}
		else if (event.GetEventType() == Engine::EventType::MouseScrolled)
		{
			auto& mouseScrolledEvent = static_cast<Engine::MouseScrolledEvent&>(event);
			// Adjusting camera FOV when scrolling
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
		// Updates the camera position and recalculates the view matrix.
		SetPosition(position);
	}

	void PerspectiveCamera::ProcessMouseMovement(float xOffset, float yOffset)
	{
		if (!m_MouseEnabled)
			return;

		// This is to invert the mouse movement
		xOffset *= m_MouseSensitivity;
		yOffset *= m_MouseSensitivity;
		// Updates yaw and pitch angles
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
		// Calculate direction vector from yaw and pitch
		glm::vec3 front{};
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Front = glm::normalize(front);

		// Calculates the direction vector to the right and above the camera.
		m_Right = glm::normalize(glm::cross(m_Front, glm::vec3(0.0f, 1.0f, 0.0f)));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));
		
		// Generate view matrix using glm::lookAt
		glm::mat4 view = glm::lookAt(m_Position, m_Position + m_Front, m_Up);

		// Applies rotation (roll) to view matrix
		if (m_Roll != 0.0f)
		{
			glm::mat4 rollMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(m_Roll), m_Front);
			view = rollMatrix * view;
		}

		// Updates view matrix and projection matrix
		m_ViewMatrix = view;
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}