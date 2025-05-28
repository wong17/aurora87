
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"Player/Player.h"
#include"Base/EntityManager.h"
#include"Render/shaderClass.h"
#include"Render/Camera.h"
#include"cmath"
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include "DumbZombie/DumbZombie.h"
#include <random>
#include "RenderEnviroment/PlayerRender.h"
#include "RenderEnviroment/RenderDumbZombie.h"
const unsigned int width = 1800;
#include <ZombieSleep.h>
#include <RenderLight/Light.h>

const unsigned int height = 900;

int main() {

	glfwInit();


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(width, height, "OpenAIGame", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();

	glViewport(0, 0, width, height);

	Shader shaderProgram("zombieDumb.vert", "zombieDumb.frag");
	Shader shaderLight("light.vert", "light.frag");

	RenderDumbZombie renderZombie;
	PlayerRender renderPlayer;
	Light renderLight;
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
	Player* player = new Player();
	renderZombie.RegisterEntities(shaderProgram, shaderLight);
	renderPlayer.RegisterEntities(shaderProgram, shaderLight);

	renderLight.RegisterEntities(shaderLight);

	EntityManager::Instance().RegisterEntity(player);
	float lastTime = glfwGetTime();
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		camera.Inputs(window, player->GetID());
		camera.Matrix(45.0f, 0.1f, 200.0f);
		player->Update();

		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		player->SetDeltaTime(deltaTime);
		renderPlayer.Render(deltaTime, shaderProgram);
		renderZombie.Render(deltaTime, shaderProgram);
		camera.ShaderMatrix(shaderProgram, "camMatrix");
		renderLight.Render(deltaTime, shaderLight);
		camera.ShaderMatrix(shaderLight, "camMatrix");

		glfwSwapBuffers(window);
	}
	shaderProgram.Delete();
	shaderLight.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	renderPlayer.CLoseBuffer();
	renderZombie.CloseBuffer();
	renderLight.CLoseBuffer();
	delete player;
	return 0;
}