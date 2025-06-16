#include "Skybox.h"

namespace Engine
{
	Skybox::Skybox(const std::vector<std::string>& faces, std::shared_ptr<Shader> shader)
		: m_Shader(std::move(shader))
	{
		InitVertexData();
		m_Texture = std::make_shared<CubemapTexture>(faces);
	}

	void Skybox::Draw(const glm::mat4& view, const glm::mat4& projection)
	{
		auto& renderSettings = Engine::Application::Get().GetRenderSettings();

		renderSettings.SetCullFaceMode(Engine::CullFaceMode::Front);
		renderSettings.SetDepthFunc(Engine::DepthFunc::Lequal);

		m_Shader->Bind();
		// we eliminate the view translation component so that the skybox always remains “fixed”.
		glm::mat4 skyView = glm::mat4(glm::mat3(view));
		m_Shader->SetMat4("u_ViewMatrix", skyView);
		m_Shader->SetMat4("u_ProjectionMatrix", projection);

		m_VertexArray->Bind();
		m_Texture->Bind();
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_VertexBuffer->GetVertexCount()));

		renderSettings.SetCullFaceMode(Engine::CullFaceMode::Back);
		renderSettings.SetDepthFunc(Engine::DepthFunc::Less);
	}

	void Skybox::SetProjectionMatrix(const glm::mat4& projection)
	{
		m_Shader->Bind();
		m_Shader->SetMat4("u_ProjectionMatrix", projection);
	}

	void Skybox::InitVertexData()
	{
		float vertices[] =
		{
			// Top face
			 1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f,  1.0f,
			 1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f,  1.0f,
			 1.0f, 1.0f,  1.0f,

			// Front face
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			// Left face
			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,

			// Bottom face
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			// Right face
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,

			// Back face
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f
		};

		m_VertexBuffer = std::make_shared<VertexBuffer>(vertices, static_cast<uint32_t>(sizeof(vertices)));
		VertexBufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" }
		};
		m_VertexBuffer->SetLayout(layout);

		m_VertexArray = std::make_shared<VertexArray>();
		m_VertexArray->Bind();
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
	}
}