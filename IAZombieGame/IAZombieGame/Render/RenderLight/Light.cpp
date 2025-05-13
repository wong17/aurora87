#include "Light.h"
#include <random>


static GLfloat Vertices[] = {
	//// Cara trasera (Back)
	-0.5f, -0.5f, -0.5f,   1, 0, 0,   0.0f, 0.0f, // bottom-left
	 0.5f, -0.5f, -0.5f,   0, 1, 0,   1.0f, 0.0f, // bottom-right
	 0.5f,  0.5f, -0.5f,   0, 0, 1,   1.0f, 1.0f, // top-right
	-0.5f,  0.5f, -0.5f,   1, 1, 0,   0.0f, 1.0f, // top-left

	-0.5f, -0.5f,  0.5f,   0, 1, 1,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,   1, 0, 1,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,   0.5,0.5,0.5, 1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,   1, 1, 1,   0.0f, 1.0f,

	//// Cara izquierda (Left)
	-0.5f, -0.5f,  0.5f,   1, 0, 0,   0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,   0, 1, 0,   1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,   0, 0, 1,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,   1, 1, 0,   0.0f, 1.0f,

	//// Cara derecha (Right)
	 0.5f, -0.5f,  0.5f,   0, 1, 1,   0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,   1, 0, 1,   1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,   0.5,0.5,0.5, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,   1, 1, 1,   0.0f, 1.0f,

	 // // Cara inferior (Bottom)
	  -0.5f, -0.5f, -0.5f,   1, 0, 0,   0.0f, 0.0f,
	   0.5f, -0.5f, -0.5f,   0, 1, 0,   1.0f, 0.0f,
	   0.5f, -0.5f,  0.5f,   0, 0, 1,   1.0f, 1.0f,
	  -0.5f, -0.5f,  0.5f,   1, 1, 0,   0.0f, 1.0f,

	  // // Cara superior (Top)
	   -0.5f,  0.5f, -0.5f,   0, 1, 1,   0.0f, 0.0f,
		0.5f,  0.5f, -0.5f,   1, 0, 1,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,   0.5,0.5,0.5, 1.0f, 1.0f,
	   -0.5f,  0.5f,  0.5f,   1, 1, 1,   0.0f, 1.0f
};



static GLuint Indices[] = {
		0, 1, 2, 2, 3, 0,        // back
		4, 5, 6, 6, 7, 4,        // front
		8, 9,10,10,11, 8,        // left
	   12,13,14,14,15,12,        // right
	   16,17,18,18,19,16,        // bottom
	   20,21,22,22,23,20         // top

};
Light::Light() :
	lightTextures(nullptr)
	, m_count(5)
	, vbo(nullptr)
	, ebo(nullptr)
	, vao(nullptr)

{
}

Light::~Light()
{
}

void Light::RegisterEntities(Shader& shader)
{
	const char* textures[] = { "Lava_Texture.png","Lava_Texture.png","Lava_Texture.png" };
	lightTextures = new Texture(textures, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	lightTextures->texUnit(shader, "tex0", 0);
	lightTextures->texUnit(shader, "tex1", 1);
	lightTextures->texUnit(shader, "tex2", 2);
	vao = new VAO;
	vao->Bind();

	vbo = new VBO(Vertices, sizeof(Vertices));
	ebo = new EBO(Indices, sizeof(Indices));

	vao->LinkAttrib(*vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	vao->LinkAttrib(*vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	vao->LinkAttrib(*vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	vao->Unbind();
	vbo->Unbind();
	ebo->Unbind();
}

void Light::RemoveEnties()
{
}

void Light::Render(float deltaTime, Shader& shader)
{
	unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");

	int activeTex = 0;
	GLuint activeTexLoc = glGetUniformLocation(shader.ID, "figureID");
	glUniform1i(activeTexLoc, activeTex);
	shader.Activate();

	vao->Bind();

	
	for (int i = 2; i < GetCount() + 4; i++) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f , 1.0f * i , 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		lightTextures->Bind(0);
	
		glDrawElements(GL_TRIANGLES, sizeof(Indices) / sizeof(int), GL_UNSIGNED_INT, 0);
		lightTextures->Unbind();
	}
	for (int i = 2; i < GetCount() + 3; i++) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f , 1.0f * i , 1.0f * i));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		lightTextures->Bind(0);
	
		glDrawElements(GL_TRIANGLES, sizeof(Indices) / sizeof(int), GL_UNSIGNED_INT, 0);
		lightTextures->Unbind();
	}
	for (int i = 2; i < GetCount() + 3; i++) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f  , 8.0f , 1.0f * i));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		lightTextures->Bind(0);
	
		glDrawElements(GL_TRIANGLES, sizeof(Indices) / sizeof(int), GL_UNSIGNED_INT, 0);
		lightTextures->Unbind();
	}
}

void Light::HandleEvent(const EventData& eventData)
{
}

void Light::Delete()
{
}

void Light::CLoseBuffer()
{
	vao->Delete();
	ebo->Delete();
	vbo->Delete();
}
