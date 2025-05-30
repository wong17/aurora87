#include "GameCameraBeta.h"
#include <unordered_set>


namespace Engine {

	CameraGameBeta::CameraGameBeta(int width, int height, glm::vec3 position)
	{
		CameraGameBeta::width = width;
		CameraGameBeta::height = height;
		Position = position;
		playerID = -1;
		count = 0;
		AIEnviroment::EventManager::GetInstance()->AddListener(AIEnviroment::EventType::RANDOM_SPAWN, this);
		AIEnviroment::EventManager::GetInstance()->AddListener(AIEnviroment::EventType::PLAYER_RESPAWN, this);
		AIEnviroment::EventManager::GetInstance()->AddListener(AIEnviroment::EventType::PLAYER_MOVE, this);
	}

	void CameraGameBeta::OnEvent(Event& event)
	{
	}

	void CameraGameBeta::ProcessKeyboard(CameraMovement movement, float deltaTime)
	{
	}

	void CameraGameBeta::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch)
	{
	}

	void CameraGameBeta::SetProjection(float fov, float aspectRatio, float nearClip, float farClip)
	{
	}

	glm::vec3 CameraGameBeta::NormalizeCameraPos(glm::vec3 pos, float minVal, float maxVal)
	{
		return (pos - glm::vec3(minVal)) / (maxVal - minVal) * 2.0f - glm::vec3(1.0f);
	}

	void CameraGameBeta::SetRotation(const glm::vec3& rotation)
	{
	}

	void CameraGameBeta::MoveEvent(glm::vec3 Position)
	{
		//glm::vec3 position = NormalizeCameraPos(Position, -1000.0f, 1000.0f);
		AIEnviroment::EventData moveEvent(AIEnviroment::EventType::PLAYER_MOVE);
		moveEvent.Set("newPositionX", Position.x);
		moveEvent.Set("newPositiony", Position.y);
		moveEvent.Set("newPositionz", Position.z);
		moveEvent.Set("playerMovedID", playerID);
		AIEnviroment::EventManager::GetInstance()->DispatchEvent(moveEvent);
	}

	void CameraGameBeta::Matrix(float FOVdeg, float nearPlane, float farPlane)
	{

		// Initializes matrices since otherwise they will be the null matrix
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		// Makes camera look in the right direction from the right position
		view = glm::lookAt(Position, Position + Orientation, Up);
		// Adds perspective to the scene
		projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);
		View = view;
		Projection = projection;
		matrixCamera = projection * view;
		m_ProjectionMatrix = projection;
		m_ViewMatrix = view;
		m_ViewProjectionMatrix = matrixCamera;
		// Exports the camera matrix to the Vertex Shader
	}



	void CameraGameBeta::Inputs(GLFWwindow* window, int id)
	{
		playerID = id;

		// Handles key inputs
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			Position += speed * Orientation;
			MoveEvent(Position);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			Position += speed * -glm::normalize(glm::cross(Orientation, Up));
			MoveEvent(Position);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			Position += speed * -Orientation;
			MoveEvent(Position);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			Position += speed * glm::normalize(glm::cross(Orientation, Up));
			MoveEvent(Position);
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			Position += speed * Up;
			MoveEvent(Position);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			Position += speed * -Up;
			MoveEvent(Position);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			speed = 0.7f;
			AIEnviroment::EventData moveEvent(AIEnviroment::EventType::PLAYER_SPEED);
			moveEvent.Set("playerMovedID", id);
			AIEnviroment::EventManager::GetInstance()->DispatchEvent(moveEvent);

		}
		else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		{
			speed = 0.1f;
		}
		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			hidden = false;
		}
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			hidden = true;
		}
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		{
			glfwMaximizeWindow(window);
		}
		if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		{
			glfwRestoreWindow(window);

		}
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		{
			brickTexture = true;
			EventData BrickTexture(AIEnviroment::EventType::CUBE_TEXTURE);
			BrickTexture.Set("brickTexture", brickTexture);
			BrickTexture.Set("lavaTexture", false);
			BrickTexture.Set("grassTexture", false);
			BrickTexture.Set("waterTexture", false);
			BrickTexture.Set("woodTexture", false);
			EventManager::GetInstance()->DispatchEvent(BrickTexture);
		}
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		{
			lavaTexture = true;
			EventData LavaTexture(AIEnviroment::EventType::CUBE_TEXTURE);
			LavaTexture.Set("brickTexture", false);
			LavaTexture.Set("lavaTexture", lavaTexture);
			LavaTexture.Set("grassTexture", false);
			LavaTexture.Set("waterTexture", false);
			LavaTexture.Set("woodTexture", false);
			EventManager::GetInstance()->DispatchEvent(LavaTexture);
		}
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			EventData gunZeroIndex(AIEnviroment::EventType::PLAYER_CHANGE_GUN);
			gunZeroIndex.Set("GunIndex", 0);
			EventManager::GetInstance()->DispatchEvent(gunZeroIndex);

		}
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		{
			EventData gunFirstIndex(AIEnviroment::EventType::PLAYER_CHANGE_GUN);
			gunFirstIndex.Set("GunIndex", 1);
			EventManager::GetInstance()->DispatchEvent(gunFirstIndex);

		}
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		{
			EventData gunZeroIndex(AIEnviroment::EventType::PLAYER_CHANGE_GUN);
			gunZeroIndex.Set("GunIndex", 2);
			EventManager::GetInstance()->DispatchEvent(gunZeroIndex);

		}
		if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		{
			EventData gunThirdIndex(AIEnviroment::EventType::PLAYER_CHANGE_GUN);
			gunThirdIndex.Set("GunIndex", 3);
			EventManager::GetInstance()->DispatchEvent(gunThirdIndex);

		}	
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		{
			EventData gunReload(AIEnviroment::EventType::RELOADING);
			EventManager::GetInstance()->DispatchEvent(gunReload);

		}

		if (hidden) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

			// Stores the coordinates of the cursor
			double mouseX;
			double mouseY;
			// Fetches the coordinates of the cursor
			glfwGetCursorPos(window, &mouseX, &mouseY);

			// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
			// and then "transforms" them into degrees 
			float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
			float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

			// Calculates upcoming vertical change in the Orientation
			glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

			// Decides whether or not the next vertical Orientation is legal or not
			if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
			{
				Orientation = newOrientation;
			}

			// Rotates the Orientation left and right
			Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);
			EventData orientationEvent(AIEnviroment::EventType::PLAYER_ROTATE);
			orientationEvent.Set("NewOrientationx", Orientation.x);
			orientationEvent.Set("NewOrientationy", Orientation.y);
			orientationEvent.Set("NewOrientationz", Orientation.z);
			orientationEvent.Set("playerIDRotate", id);
			EventManager::GetInstance()->DispatchEvent(orientationEvent);
			// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
			glfwSetCursorPos(window, (width / 2), (height / 2));
		}
		int mouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		if (mouseState == GLFW_PRESS && !clickLastFrame)
		{

			EventData attackEvent(AIEnviroment::EventType::PLAYER_ATTACK);
			attackEvent.Set("playerIDAttack", id);

			glm::vec3 pointInFront = Position + glm::normalize(Orientation) * 20.0f;
			attackEvent.Set("clickX", pointInFront.x);
			attackEvent.Set("clickY", pointInFront.y);
			attackEvent.Set("clickZ", pointInFront.z);

			std::cout << "Click en: X=" << pointInFront.x << ", Y=" << pointInFront.y << ", Z=" << pointInFront.z << std::endl;
			std::cout << "Se activa evento" << std::endl;


			EventManager::GetInstance()->DispatchEvent(attackEvent);


		}

		clickLastFrame = (mouseState == GLFW_PRESS);

	}

	void CameraGameBeta::HandleEvent(const EventData& event)
	{
		if (event.type == AIEnviroment::EventType::RANDOM_SPAWN)
		{
			int playerIDs = event.GetInt("playerRandomSpawnID");
			if (playerID == playerIDs)
			{
				float newX = event.GetFloat("newPositionX");
				float newY = event.GetFloat("newPositionY");
				float newZ = event.GetFloat("newPositionZ");
				std::cout << "Camara position" << std::endl;
				std::cout << newX << std::endl;
				std::cout << newY << std::endl;
				std::cout << newZ << std::endl;
				Position = glm::vec3(newX, newY, newZ);
			}

		}
	}

	void CameraGameBeta::RecalculateViewMatrix()
	{
	}

}