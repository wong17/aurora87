#pragma once

#include "Engine/Core/Application.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Events/EventTypes.h"
#include "Camera.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <algorithm>

namespace Engine
{
	enum class CameraMovement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void SetProjection(float fov, float aspectRatio, float nearClip, float farClip) override;

		void SetRotation(const glm::vec3& rotation) override;

		void SetYaw(float yaw);
		void SetPitch(float pitch);
		void SetRoll(float roll);

		void SetFOV(float fov);
		void AdjustFOV(float deltaFOV, float minFOV = 1.0f, float maxFOV = 120.0f);
		
		void SetAspectRatio(float aspectRatio);
		void SetNearClip(float nearClip);
		void SetFarClip(float farClip);
	
		inline void SetMouseSensitivity(float sens) { m_MouseSensitivity = sens; }
		inline void SetMovementSpeed(float speed) { m_MovementSpeed = speed; }

		inline void EnableKeyboard(bool enabled) { m_KeyboardEnabled = enabled; }
		inline void EnableMouse(bool enabled) 
		{
			m_MouseEnabled = enabled;
			if (!enabled) m_FirstMouse = true;
		}

		inline void SetYawLimits(float minYaw, float maxYaw) 
		{
			m_MinYaw = minYaw;
			m_MaxYaw = maxYaw;
			m_YawLimitsEnabled = true;
			m_Yaw = std::clamp(m_Yaw, m_MinYaw, m_MaxYaw);
			RecalculateViewMatrix();
		}

		inline void ResetYawLimits() 
		{
			m_YawLimitsEnabled = false;
		}

		inline void SetPitchLimits(float minPitch, float maxPitch) 
		{
			m_MinPitch = minPitch;
			m_MaxPitch = maxPitch;
			m_Pitch = std::clamp(m_Pitch, m_MinPitch, m_MaxPitch);
			RecalculateViewMatrix();
		}

		inline void ResetPitchLimits() 
		{
			SetPitchLimits(-89.0f, 89.0f);
		}

		inline const glm::vec3& GetFront() const { return m_Front; }
		inline const glm::vec3& GetRight() const { return m_Right; }
		inline const glm::vec3& GetUp() const { return m_Up; }

		inline float GetYaw() const { return m_Yaw; }
		inline float GetPitch() const { return m_Pitch; }
		inline float GetRoll() const { return m_Roll; }

		inline float GetFOV() const { return m_FOV; }
		inline float GetAspectRatio() const { return m_AspectRatio; }
		inline float GetNearClip() const { return m_NearClip; }
		inline float GetFarClip() const { return m_FarClip; }

		inline const glm::mat4& GetViewMatrix() const override { return m_ViewMatrix; }
		inline const glm::mat4& GetProjectionMatrix() const override { return m_ProjectionMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const override { return m_ViewProjectionMatrix; }

		void OnEvent(Event& event) override;
		void ProcessKeyboard(CameraMovement movement, float deltaTime);
		void ProcessMouseMovement(float xOffset, float yOffset);

	protected:
		void RecalculateViewMatrix() override;
	protected:
		glm::vec3 m_Front = { 0.0f, 0.0f, -1.0f };	// Indica la dirección hacia la que está mirando la cámara
		glm::vec3 m_Right = { 1.0f, 0.0f, 0.0f };	// Indica la dirección a la derecha de la cámara
		glm::vec3 m_Up = { 0.0f, 1.0f, 0.0f };		// Indica la dirección hacia arriba de la cámara

		float m_FOV, m_AspectRatio, m_NearClip, m_FarClip;
		/* Con yaw = –90° y pitch = 0°, el vector de dirección resultante es (0, 0, –1), lo que alinea la vista de la cámara con el eje Z negativo. */
		float m_Yaw = -90.0f;	// Rotación inicial en el eje Y en grados (plano horizontal)
		float m_Pitch = 0.0f;	// Rotación inicial en el eje X en grados (plano vertical)
		float m_Roll = 0.0f;	// Rotación inicial en el eje Z en grados (plano de profundidad) 

		float m_MinYaw = 0.0f;
		float m_MaxYaw = 360.0f;
		float m_MinPitch = -89.f;
		float m_MaxPitch = 89.f;

		bool m_YawLimitsEnabled = false;

		float m_MouseSensitivity = 0.1f;	// Sensibilidad del mouse para el movimiento de la cámara
		float m_MovementSpeed = 100.0f;		// Velocidad de movimiento de la cámara (en unidades por segundo)

		bool  m_KeyboardEnabled = true;
		bool  m_MouseEnabled = true;

		float m_LastX = 0.0f;
		float m_LastY = 0.0f;
		bool m_FirstMouse = true;
		bool m_MouseCaptured = true;
	};
}