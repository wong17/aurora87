#include "PlayerRender.h"
#include "RenderDumbZombie.h"

static GLfloat Vertices[] = {
	// Cara trasera (Back)
	-0.5f, -0.5f, -0.5f,   1, 0, 0,   0.0f, 0.0f,   0.0f,  0.0f, -1.0f, // bottom-left
	 0.5f, -0.5f, -0.5f,   0, 1, 0,   1.0f, 0.0f,   0.0f,  0.0f, -1.0f, // bottom-right
	 0.5f,  0.5f, -0.5f,   0, 0, 1,   1.0f, 1.0f,   0.0f,  0.0f, -1.0f, // top-right
	-0.5f,  0.5f, -0.5f,   1, 1, 0,   0.0f, 1.0f,   0.0f,  0.0f, -1.0f, // top-left

	-0.5f, -0.5f,  0.5f,   0, 1, 1,   0.0f, 0.0f,   0.0f,  0.0f,  1.0f, // bottom-left
	 0.5f, -0.5f,  0.5f,   1, 0, 1,   1.0f, 0.0f,   0.0f,  0.0f,  1.0f, // bottom-right
	 0.5f,  0.5f,  0.5f,   0.5, 0.5, 0.5,  1.0f, 1.0f,   0.0f,  0.0f,  1.0f, // top-right
	-0.5f,  0.5f,  0.5f,   1, 1, 1,   0.0f, 1.0f,   0.0f,  0.0f,  1.0f, // top-left

	// Cara izquierda (Left)
	-0.5f, -0.5f,  0.5f,   1, 0, 0,   0.0f, 0.0f,  -1.0f,  0.0f,  0.0f, // bottom-left
	-0.5f, -0.5f, -0.5f,   0, 1, 0,   1.0f, 0.0f,  -1.0f,  0.0f,  0.0f, // bottom-right
	-0.5f,  0.5f, -0.5f,   0, 0, 1,   1.0f, 1.0f,  -1.0f,  0.0f,  0.0f, // top-right
	-0.5f,  0.5f,  0.5f,   1, 1, 0,   0.0f, 1.0f,  -1.0f,  0.0f,  0.0f, // top-left

	// Cara derecha (Right)
	 0.5f, -0.5f,  0.5f,   0, 1, 1,   0.0f, 0.0f,   1.0f,  0.0f,  0.0f, // bottom-left
	 0.5f, -0.5f, -0.5f,   1, 0, 1,   1.0f, 0.0f,   1.0f,  0.0f,  0.0f, // bottom-right
	 0.5f,  0.5f, -0.5f,   0.5, 0.5, 0.5,  1.0f, 1.0f,   1.0f,  0.0f,  0.0f, // top-right
	 0.5f,  0.5f,  0.5f,   1, 1, 1,   0.0f, 1.0f,   1.0f,  0.0f,  0.0f, // top-left

	 // Cara inferior (Bottom)
	 -0.5f, -0.5f, -0.5f,   1, 0, 0,   0.0f, 0.0f,   0.0f, -1.0f,  0.0f, // bottom-left
	  0.5f, -0.5f, -0.5f,   0, 1, 0,   1.0f, 0.0f,   0.0f, -1.0f,  0.0f, // bottom-right
	  0.5f, -0.5f,  0.5f,   0, 0, 1,   1.0f, 1.0f,   0.0f, -1.0f,  0.0f, // top-right
	 -0.5f, -0.5f,  0.5f,   1, 1, 0,   0.0f, 1.0f,   0.0f, -1.0f,  0.0f, // top-left

	 // Cara superior (Top)
	 -0.5f,  0.5f, -0.5f,   0, 1, 1,   0.0f, 0.0f,   0.0f,  1.0f,  0.0f, // bottom-left
	  0.5f,  0.5f, -0.5f,   1, 0, 1,   1.0f, 0.0f,   0.0f,  1.0f,  0.0f, // bottom-right
	  0.5f,  0.5f,  0.5f,   0.5, 0.5, 0.5,  1.0f, 1.0f,   0.0f,  1.0f,  0.0f, // top-right
	 -0.5f,  0.5f,  0.5f,   1, 1, 1,   0.0f, 1.0f,   0.0f,  1.0f,  0.0f  // top-left

};



static GLuint Indices[] = {
		0, 1, 2, 2, 3, 0,        // back
		4, 5, 6, 6, 7, 4,        // front
		8, 9,10,10,11, 8,        // left
	   12,13,14,14,15,12,        // right
	   16,17,18,18,19,16,        // bottom
	   20,21,22,22,23,20 ,        // top


};

PlayerRender::PlayerRender() :
	m_count(9)
	, playerTextures(nullptr)
	,vbo(nullptr)
	,vao(nullptr)
	,ebo(nullptr)
{
	EventManager::GetInstance()->AddListener(EventType::ZOMBIE_SPAWN, this);
}

PlayerRender::~PlayerRender()
{
	EventManager::GetInstance()->RemoveListener(EventType::ZOMBIE_SPAWN, this);

}

void PlayerRender::RegisterEntities(Shader& shader, Shader& shaderlight)
{
 
	const char* textures[] = { "Steve.png","Steve.png","Steve.png"};
	playerTextures = new Texture(textures, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	playerTextures->texUnit(shader, "tex0", 0);
	playerTextures->texUnit(shader, "tex1", 1);
	playerTextures->texUnit(shader, "tex2", 2);
	vao = new VAO;
	vao->Bind();

	vbo = new VBO(Vertices, sizeof(Vertices));
	ebo = new EBO(Indices, sizeof(Indices));
	vao->LinkAttrib(*vbo, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);                   // Posición
	vao->LinkAttrib(*vbo, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float))); // Color
	vao->LinkAttrib(*vbo, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float))); // Textura
	vao->LinkAttrib(*vbo, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float))); // Normal

	vao->Unbind();
	vbo->Unbind();
	ebo->Unbind();


	glm::vec4 lightColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(5.0f, 5.0f, 4.0f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 PlayerPos = glm::vec3(0.0f, 20.0f, -1.0f);
	glm::mat4 PlayerModel = glm::mat4(1.0f);
	PlayerModel = glm::translate(PlayerModel, PlayerPos);

	shaderlight.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderlight.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(shaderlight.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    shader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(PlayerModel));
	glUniform4f(glGetUniformLocation(shader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


	std::vector<Player*> vectorEntities;
	for (int i = 0; i < GetCount(); i++) {
		Player* zombie = new Player();
		glm::vec3 zombiePos = glm::vec3(i * 1.0f, -1.0f, 0.0f);
		glm::mat4 zombieModel = glm::mat4(1.0f);


		zombieModel = glm::translate(zombieModel, zombiePos);
		zombie->SetPosition(glm::vec3(zombiePos));
		int texIndex = i % 3;  
		zombie->SetTextureID(texIndex);  

		EntityManager::Instance().RegisterEntity(zombie);
		vectorEntities.push_back(zombie);
	}	

	SetVectorPlayerEntities(vectorEntities);
}



void PlayerRender::RemoveEnties()
{
}

void PlayerRender::Render(float deltaTime, Shader& shader)
{
	unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");

	shader.Activate();

	int activeTex = 1;
	GLuint activeTexLoc = glGetUniformLocation(shader.ID, "figureID");
	glUniform1i(activeTexLoc, activeTex);
	vao->Bind();


	for (Player* p : GetPlayerEntities()) {
		if (EntityManager::Instance().GetEntity(p->GetID())) {
			p->SetDeltaTime(deltaTime);
			p->Update();

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, p->GetPosition());
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			playerTextures->Bind(p->GetTextureID());  
			glDrawElements(GL_TRIANGLES, sizeof(Indices)/sizeof(int), GL_UNSIGNED_INT, 0);
			playerTextures->Unbind();
		}
	}
}


void PlayerRender::HandleEvent(const EventData& eventData)
{
	if (eventData.type == EventType::ZOMBIE_SPAWN) {
		SetCount(eventData.GetInt("CountEntities"));
	}
}

void PlayerRender::Delete()
{
	for (Player* p : GetPlayerEntities()) {
		delete p;
	}
}

void PlayerRender::CLoseBuffer()
{
	vao->Delete();
	ebo->Delete();
	vbo->Delete();
}

