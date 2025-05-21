#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "TextureManager.h"
#include "Engine/Core/DebugGL.h"
#include "Engine/Core/Application.h"
#include "Engine/Core/RenderSettings.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstdint>
#include <string>
#include <memory>
#include <stdexcept>

namespace Engine
{
	// Configuración de malla para el domo
	struct SkydomeConfig
	{
		uint32_t Longitudes = 64;
		uint32_t Latitudes = 32;
		float Radius = 500.0f;
		float RotationSpeed = 0.01f;    // vueltas por segundo o rad/s
		float VerticalOffset = 0.0f;	// indica la altura del domo respecto al origen
	};

	class Skydome
	{
	public:
		Skydome(const std::string& equirectangularMapPath, 
			const std::string& vertexShaderPath, 
			const std::string& fragmentShaderPath, 
			const SkydomeConfig& cfg = {});
		// timeSeconds tiempo en segundos (es para animaciones, opcional)
		void Draw(const glm::mat4& view, const glm::mat4& proj, float timeSeconds = 0.0f);
		void SetViewMatrix(const glm::mat4& view) { m_Shader->SetMat4("u_ViewMatrix", view); }
		void SetProjectionMatrix(const glm::mat4& proj) { m_Shader->SetMat4("u_ProjectionMatrix", proj); }
		void SetConfig(const SkydomeConfig& cfg) { m_Config = cfg; }
		void SetTexture(const std::shared_ptr<Texture>& texture) { m_Texture = texture; }

		const SkydomeConfig& GetConfig() const { return m_Config; }
		const std::shared_ptr<Texture>& GetTexture() const { return m_Texture; }

	private:
		void LoadTexture(const std::string& path);
		void BuildHemisphere();

		SkydomeConfig m_Config;
		std::shared_ptr<Texture> m_Texture;
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<ElementBuffer> m_ElementBuffer;
		size_t m_IndexCount = 0;
	};
}