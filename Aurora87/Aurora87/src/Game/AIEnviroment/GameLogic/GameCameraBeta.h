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

	class CameraGameBeta : public EventListener, public Camera
	{
	public:
		// Stores the main vectors of the camera
		glm::vec3 Position;
		glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::mat4 matrixCamera = glm::mat4(1.0f);
		glm::mat4 View = glm::mat4(1.0f);
		glm::mat4 Projection = glm::mat4(1.0f);
		// Prevents the camera from jumping around when first clicking left click
		bool m_FirstMouse = true;
		bool m_MouseCaptured = true;
		// Stores the width and height of the window
		int width;
		int height;
		int count;
		bool clickLastFrame = false;
		bool hidden = true;
		bool clicked = false;
		bool brickTexture = false;
		bool lavaTexture = false;
		// Adjust the speed of the camera and it's sensitivity when looking around
		float speed = 0.2f;
		float sensitivity = 80.0f;

		// Camera constructor to set up initial values
		CameraGameBeta(int width, int height, glm::vec3 position);

		// Updates and exports the camera matrix to the Vertex Shader
		void OnEvent(Event& event) override;
		void ProcessKeyboard(CameraMovement movement, float deltaTime);
		void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
		void SetProjection(float fov, float aspectRatio, float nearClip, float farClip) override;
		glm::vec3 NormalizeCameraPos(glm::vec3 pos, float minVal, float maxVal);
		void SetRotation(const glm::vec3& rotation) override;

		inline const glm::mat4& GetViewMatrix() const override { return m_ViewMatrix; }
		inline const glm::mat4& GetProjectionMatrix() const override { return m_ProjectionMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const override { return m_ViewProjectionMatrix; }

		void MoveEvent(glm::vec3 Position);

		void Matrix(float FOVdeg, float nearPlane, float farPlane);
		// Handles camera inputs
		void Inputs(GLFWwindow* window, int id);
		void HandleEvent(const EventData& event) override;
		int playerID;
	protected:
		void RecalculateViewMatrix() override;
	};

}