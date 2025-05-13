#include"Camera.h"
#include "EventManager.h"
#include "EventData.h"
Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
	playerID = -1;
	count = 0;
	EventManager::GetInstance()->AddListener(EventType::RANDOM_SPAWN, this);
	EventManager::GetInstance()->AddListener(EventType::PLAYER_RESPAWN, this);
	EventManager::GetInstance()->AddListener(EventType::PLAYER_MOVE, this);
}


void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane)
{

	// Initializes matrices since otherwise they will be the null matrix
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Makes camera look in the right direction from the right position
	view = glm::lookAt(Position, Position + Orientation, Up);
	// Adds perspective to the scene
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	matrixCamera = projection * view;
	// Exports the camera matrix to the Vertex Shader
}

void Camera::ShaderMatrix(Shader& shader, const char* uniform)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(matrixCamera));

}

void Camera::Inputs(GLFWwindow* window, int id)
{
	playerID = id;

	// Handles key inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += speed * Orientation;
		EventData moveEvent(EventType::PLAYER_MOVE);
		moveEvent.Set("newPositionX", Position.x);
		moveEvent.Set("newPositiony", Position.y);
		moveEvent.Set("newPositionz", Position.z);
		moveEvent.Set("playerMovedID", id);
		EventManager::GetInstance()->DispatchEvent(moveEvent);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position += speed * -glm::normalize(glm::cross(Orientation, Up));
		EventData moveEvent(EventType::PLAYER_MOVE);
		moveEvent.Set("newPositionX", Position.x);
		moveEvent.Set("newPositiony", Position.y);
		moveEvent.Set("newPositionz", Position.z);
		moveEvent.Set("playerMovedID", id);
		EventManager::GetInstance()->DispatchEvent(moveEvent);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position += speed * -Orientation;
		EventData moveEvent(EventType::PLAYER_MOVE);
		moveEvent.Set("newPositionX", Position.x);
		moveEvent.Set("newPositiony", Position.y);
		moveEvent.Set("newPositionz", Position.z);
		moveEvent.Set("playerMovedID", id);
		EventManager::GetInstance()->DispatchEvent(moveEvent);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += speed * glm::normalize(glm::cross(Orientation, Up));
		EventData moveEvent(EventType::PLAYER_MOVE);
		moveEvent.Set("newPositionX", Position.x);
		moveEvent.Set("newPositiony", Position.y);
		moveEvent.Set("newPositionz", Position.z);
		moveEvent.Set("playerMovedID", id);
		EventManager::GetInstance()->DispatchEvent(moveEvent);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Position += speed * Up;
		EventData moveEvent(EventType::PLAYER_MOVE);
		moveEvent.Set("newPositionX", Position.x);
		moveEvent.Set("newPositiony", Position.y);
		moveEvent.Set("newPositionz", Position.z);
		moveEvent.Set("playerMovedID", id);
		EventManager::GetInstance()->DispatchEvent(moveEvent);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Position += speed * -Up;
		EventData moveEvent(EventType::PLAYER_MOVE);
		moveEvent.Set("newPositionX", Position.x);
		moveEvent.Set("newPositiony", Position.y);
		moveEvent.Set("newPositionz", Position.z);
		moveEvent.Set("playerMovedID", playerID);
		EventManager::GetInstance()->DispatchEvent(moveEvent);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = 0.4f;
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
		EventData BrickTexture(EventType::CUBE_TEXTURE);
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
		EventData LavaTexture(EventType::CUBE_TEXTURE);
		LavaTexture.Set("brickTexture", false);
		LavaTexture.Set("lavaTexture", lavaTexture);
		LavaTexture.Set("grassTexture", false);
		LavaTexture.Set("waterTexture", false);
		LavaTexture.Set("woodTexture", false);
		EventManager::GetInstance()->DispatchEvent(LavaTexture);
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
		EventData orientationEvent(EventType::PLAYER_ROTATE);
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

		EventData attackEvent(EventType::PLAYER_ATTACK);
		attackEvent.Set("playerIDAttack", id);

		glm::vec3 pointInFront = Position + glm::normalize(Orientation) * 2.0f;
		attackEvent.Set("clickX", pointInFront.x);
		attackEvent.Set("clickY", pointInFront.y);
		attackEvent.Set("clickZ", pointInFront.z);

		std::cout << "Click en: X=" << pointInFront.x << ", Y=" << pointInFront.y << ", Z=" << pointInFront.z << std::endl;
		std::cout << "Se activa evento" << std::endl;


		EventManager::GetInstance()->DispatchEvent(attackEvent);


	}

	 clickLastFrame = (mouseState == GLFW_PRESS);

}

void Camera::HandleEvent(const EventData& event)
{
	if (event.type == EventType::RANDOM_SPAWN)
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
