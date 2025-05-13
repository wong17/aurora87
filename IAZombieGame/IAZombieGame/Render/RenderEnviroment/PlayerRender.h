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
#include <Player.h>
#include <VAO.h>
#include <EBO.h>


class PlayerRender : public EventListener
{
private:
	std::vector<Player*> m_player;
	int m_count;	
	Texture* playerTextures;

public:
	PlayerRender();
	~PlayerRender();
	VAO* vao;
	EBO* ebo;
	VBO* vbo;
	std::vector<Player*> GetPlayerEntities() const { return m_player; }
	void SetVectorPlayerEntities(const std::vector<Player*> zombies) { m_player = zombies; }
	void SetCount(const int count) { m_count = count; }
	int GetCount() {return  m_count; }
	void RegisterEntities(Shader& shader, Shader& shaderLight);
	void RemoveEnties();
	void Render(float deltaTime, Shader& shader);
	void HandleEvent(const EventData& eventData);
	void Delete();
	void CLoseBuffer();

};

