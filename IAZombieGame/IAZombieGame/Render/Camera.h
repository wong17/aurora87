#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include "EventManager.h"
#include "EventData.h"
#include"shaderClass.h"

class Camera : public EventListener
{
public:
	// Stores the main vectors of the camera
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 matrixCamera = glm::mat4(1.0f);
	// Prevents the camera from jumping around when first clicking left click
	bool hidden = true;
	bool clicked = false;
	// Stores the width and height of the window
	int width;
	int height;
	int count;
	bool clickLastFrame = false;

	bool brickTexture = false;
	bool lavaTexture = false;
	// Adjust the speed of the camera and it's sensitivity when looking around
	float speed = 0.1f;
	float sensitivity = 80.0f;

	// Camera constructor to set up initial values
	Camera(int width, int height, glm::vec3 position);

	// Updates and exports the camera matrix to the Vertex Shader

	void Matrix(float FOVdeg, float nearPlane, float farPlane);
	void ShaderMatrix(Shader& shader, const char* uniform);
	// Handles camera inputs
	void Inputs(GLFWwindow* window, int id);
	void HandleEvent(const EventData& event);
	int playerID;
};
#endif