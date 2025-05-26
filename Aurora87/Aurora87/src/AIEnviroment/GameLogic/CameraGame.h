#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include "Engine/Core/Application.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Events/EventTypes.h"
#include <AIEnviroment/EventGame/EventData.h>
#include <AIEnviroment/EventGame/EventManager.h>
#include "Engine/OpenGL/Camera.h"

using namespace AIEnviroment;

#include <iostream>
#include <algorithm>
#include <Engine/OpenGL/Shader.h>
#include <GLFW/glfw3.h>
#include <unordered_set>


namespace Engine {


	class CameraGame : public Camera, public EventListener
	{
	public:
		CameraGame(float fov, float aspectRatio, float nearClip, float farClip);

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

		void Update(float deltaTime);

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
		void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
		void HandleEvent(const EventData& event) override;

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

		float m_MouseSensitivity = 0.1f;	// Sensibilidad del mouse para el movimiento de la cámara
		float m_MovementSpeed = 100.0f;		// Velocidad de movimiento de la cámara (en unidades por segundo)

		float m_LastX = 0.0f;
		float m_LastY = 0.0f;
		bool m_FirstMouse = true;
		bool m_MouseCaptured = true;
		std::unordered_set<int> m_KeysPressed;

	};

}