#pragma once
#include "EventData.h"
#include "EventManager.h"
#include "Base/EntityManager.h"
#include <glad/glad.h>
#include"Texture.h"
#include"shaderClass.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <VAO.h>
#include <EBO.h>
#include <Camera.h>


#pragma once
class Light
{
private: 
	int m_count;
	Texture* lightTextures;
public:
	VAO* vao;
	EBO* ebo;
	VBO* vbo;
	Light();
	~Light();

	void SetCount(const int count) { m_count = count; }
	int GetCount() { return  m_count; }
	void RegisterEntities(Shader& shader);
	void RemoveEnties();
	void Render(float deltaTime, Shader& shader);
	void HandleEvent(const EventData& eventData);
	void Delete();
	void CLoseBuffer();
};

