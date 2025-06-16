#include "Skydome.h"

namespace Engine
{
	Skydome::Skydome(const std::string& equirectangularMapPath, 
		const std::string& vertexShaderPath,
		const std::string& fragmentShaderPath, 
		const SkydomeConfig& cfg)
	{
		m_Shader = Shader::Create(vertexShaderPath, fragmentShaderPath);
		// Load equirectangular texture (lat/long)
		LoadTexture(equirectangularMapPath);
		// Generate hemisphere geometry
		BuildHemisphere();
	}

	void Skydome::Draw(const glm::mat4& view, const glm::mat4& proj, float timeSeconds)
	{
		auto& renderSettings = Engine::Application::Get().GetRenderSettings();

		renderSettings.SetDepthFunc(DepthFunc::Lequal);

		m_Shader->Bind();
		glm::mat4 skydomeView = glm::mat4(glm::mat3(view));
		m_Shader->SetMat4("u_ViewMatrix", skydomeView);
		m_Shader->SetMat4("u_ProjectionMatrix", proj);
		m_Shader->SetFloat("u_VerticalOffset", m_Config.VerticalOffset);
		m_Shader->SetFloat("u_Speed", m_Config.RotationSpeed);
		m_Shader->SetFloat("u_Time", timeSeconds);

		m_Shader->SetInt("u_SkydomeTex", 0);
		m_Texture->Bind();

		m_VertexArray->Bind();
		GLCall(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_IndexCount), GL_UNSIGNED_INT, nullptr));

		renderSettings.SetDepthFunc(DepthFunc::Less);
	}

	void Skydome::LoadTexture(const std::string& path)
	{
		// Use the TextureManager to cache and reuse textures
		m_Texture = TextureManager::Get().Load(path);
		if (!m_Texture->IsLoaded())
		{
			throw std::runtime_error("Skydome::LoadTexture: failure to load texture " + path);
		}
	}

	void Skydome::BuildHemisphere()
	{
		const uint32_t L = m_Config.Longitudes;  // horizontal divisions
		const uint32_t M = m_Config.Latitudes;   // vertical divisions (hemisphere)
		const float    R = m_Config.Radius;

		std::vector<float> rawVertices;
		std::vector<uint32_t> indices;
		// Each vertex x,y,z + u,v = 5 floats
		rawVertices.reserve((static_cast<std::vector<float, std::allocator<float>>::size_type>(L) + 1) * (static_cast<unsigned long long>(M) + 1) * 5);
		// Each square becomes 2 triangles so we multiply by 6.
		indices.reserve(static_cast<std::vector<uint32_t, std::allocator<uint32_t>>::size_type>(L) * M * 6);

		// Generate the hemisphere vertices
		for (uint32_t row = 0; row <= M; ++row)
		{
			// v ∈ [0,1] travels from Pole (v=0) to equator (v=1)
			float v = float(row) / float(M);
			// θ ∈ [0, π/2] for hemisphere
			float theta = v * (glm::half_pi<float>());
			float sinT = std::sin(theta);
			float cosT = std::cos(theta);

			for (uint32_t col = 0; col <= L; ++col) 
			{
				// u ∈ [0,1] runs through lengths 0...2π
				float u = float(col) / float(L);
				float phi = u * (glm::two_pi<float>());
				float sinP = std::sin(phi);
				float cosP = std::cos(phi);

				// 3D position on sphere
				float x = R * sinT * cosP;
				float y = R * cosT;
				float z = R * sinT * sinP;
				rawVertices.push_back(x);
				rawVertices.push_back(y);
				rawVertices.push_back(z);

				// equirectangular UV coordinates
				rawVertices.push_back(u); // map φ ∈ [0,2π] -> u ∈ [0,1].
				rawVertices.push_back(1.0f - v); // map θ ∈ [0,π/2] -> v ∈ [0,1] we invert the component y
			}
		}

		// Generate the indexes for each square (2 triangles)
		for (uint32_t row = 0; row < M; ++row) {
			for (uint32_t col = 0; col < L; ++col) {
				uint32_t i0 = row * (L + 1) + col;
				uint32_t i1 = row * (L + 1) + (col + 1);
				uint32_t i2 = (row + 1) * (L + 1) + col;
				uint32_t i3 = (row + 1) * (L + 1) + (col + 1);
				// Triangle 1: (i0, i2, i1)
				indices.push_back(i0);
				indices.push_back(i2);
				indices.push_back(i1);
				// Triangle 2: (i1, i2, i3)
				indices.push_back(i1);
				indices.push_back(i2);
				indices.push_back(i3);
			}
		}

		m_IndexCount = static_cast<uint32_t>(indices.size());

		m_VertexArray = std::make_shared<VertexArray>();
		m_VertexBuffer = std::make_shared<VertexBuffer>(rawVertices);
		m_ElementBuffer = std::make_shared<ElementBuffer>(indices);

		// Define layout: position (vec3) + uv (vec2)
		VertexBufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
		};
		m_VertexBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetElementBuffer(m_ElementBuffer);
	}
}