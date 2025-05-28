#pragma once
#include "EventData.h"
#include "EventManager.h"
#include "DumbZombie.h"
#include "Base/EntityManager.h"
#include <glad/glad.h>
#include"Texture.h"
#include"shaderClass.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <VAO.h>
#include <EBO.h>

class RenderDumbZombie : public EventListener
{
private:
	std::vector<DumbZombie*> m_zombies;
	int m_count;	
	Texture* zombieTextures;

public:
	RenderDumbZombie();
	~RenderDumbZombie();
	VAO* vao;
	EBO* ebo;
	VBO* vbo;

	std::vector<DumbZombie*> GetZombiesEntities() const { return m_zombies; }
	void SetVectorZombieEntities(const std::vector<DumbZombie*> zombies) { m_zombies = zombies; }
	void SetCount(const int count) { m_count = count; }
	int GetCount() {return  m_count; }
	void RegisterEntities(Shader& shader, Shader& shaderlight);
	void RemoveEnties();
	void Render(float deltaTime, Shader& shader);
	void HandleEvent(const EventData& eventData);
	void Delete();
	void CloseBuffer();



};

