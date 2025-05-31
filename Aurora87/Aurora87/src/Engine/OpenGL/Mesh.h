#pragma once

#include "Engine/Core/DebugGL.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "VertexBufferLayout.h"
#include "ShaderDataTypes.h"
#include "AssimpTypes.h"
#include "UniformBuffer.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>
#include <map>
#include <memory>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

namespace Engine
{
	// Clase para manejar la malla del modelo u objeto 3D primitivo
	class Mesh
	{
	public:
		Mesh(const std::vector<float>& rawVertices,
			const VertexBufferLayout& layout = DefaultLayout(),
			GLenum drawMode = GL_TRIANGLES);

		Mesh(const std::vector<float>& rawVertices,
			const std::vector<TextureData>& textures = {},
			const VertexBufferLayout& layout = TextureLayout(),
			GLenum drawMode = GL_TRIANGLES);

		Mesh(const std::vector<float>& rawVertices,
			const std::vector<unsigned int>& indices,
			const VertexBufferLayout& layout = DefaultLayout(),
			GLenum drawMode = GL_TRIANGLES);

		Mesh(const std::vector<Vertex>& vertices,
			const std::vector<TextureData>& textures = {},
			const VertexBufferLayout& layout = DefaultModelLayout(),
			GLenum drawMode = GL_TRIANGLES,
			glm::vec3 baseColor = glm::vec3(1.0f),
			float metallic = 0.0f,
			float roughness = 1.0f);

		Mesh(const std::vector<Vertex>& vertices, 
			const std::vector<unsigned int>& indices,
			const std::vector<TextureData>& textures = {}, 
			const VertexBufferLayout& layout = DefaultModelLayout(),
			GLenum drawMode = GL_TRIANGLES,
			glm::vec3 baseColor = glm::vec3(1.0f), 
			float metallic = 0.0f, 
			float roughness = 1.0f);

		// Instancing
		void SetInstanceTransforms(const std::vector<glm::mat4>& mats, const VertexBufferLayout& instanceLayout = InstanceLayout());
		void SetDrawMode(GLenum drawMode) { m_DrawMode = drawMode; }
		void DrawDepth(Shader& depthShader);
		void DrawInstancedDepth(Shader& depthShader, uint32_t instanceCount = 0);
		void Draw(Shader& shader, UniformBuffer& textureBlockUniformBuffer, uint32_t entityIndex, bool bindTextures = true);
		void DrawInstanced(Shader& shader, UniformBuffer& textureBlockUniformBuffer, uint32_t entityIndex, uint32_t instanceCount = 0, bool bindTextures = true);
		bool CheckTextureLimit(MaterialTextureType type) const;

		// Layouts por defecto

		/*
		 * OJO: 
		 * EN EL SHADER DEBEN DE COINCIDIR LOS NOMBRES DE LOS ATRIBUTOS Y DEBEN DE ESTAR EN EL MISMO ORDEN
		 * layout(location = 0) in vec3 a_Position;
		 * ...
		 * layout(location = X) in TYPE a_Name;
		 */
		static VertexBufferLayout DefaultLayout() 
		{
			return 
			{
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float3, "a_Color"}
			};
		}
		static VertexBufferLayout TextureLayout()
		{
			return
			{
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float3, "a_Normal" },
				{ ShaderDataType::Float2, "a_TexCoord" }
			};
		}
		static VertexBufferLayout DefaultModelLayout() 
		{
			return 
			{
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float3, "a_Normal"   },
				{ ShaderDataType::Float2, "a_TexCoord"},
				{ ShaderDataType::Float3, "a_Tangent"  },
				{ ShaderDataType::Float3, "a_Bitangent"}
			};
		}
		static VertexBufferLayout InstanceLayout() 
		{
			return { { ShaderDataType::Mat4, "a_InstanceTransform" } };
		}

		// Getters
		const std::vector<Vertex>& GetVertices() const { return m_Vertices; }
		const std::vector<unsigned int>& GetIndices() const { return m_Indices; }
		const std::vector<TextureData>& GetTextures() const { return m_Textures; }
		const VertexBufferLayout& GetLayout() const { return m_Layout; }
		const VertexBufferLayout& GetInstanceLayout() const { return m_InstanceLayout; }
		
		const std::shared_ptr<VertexArray>& GetVertexArray() const { return m_VertexArray; }
		const std::shared_ptr<VertexBuffer>& GetVertexBuffer() const { return m_VertexBuffer; }
		const std::shared_ptr<ElementBuffer>& GetElementBuffer() const { return m_ElementBuffer; }
		const std::shared_ptr<VertexBuffer>& GetInstanceVertexBuffer() const { return m_InstanceVertexBuffer; }
		const std::vector<glm::mat4>& GetInstanceTransforms() const { return m_InstanceTransforms; }
		
		bool HasEBO() const { return m_HasEBO; }
		bool HasInstancing() const { return m_HasInstancing; }

		unsigned int GetDrawMode() const { return m_DrawMode; }

		bool NeedsGammaCorrection() const;

		void SetTextures(const std::vector<TextureData>& textures, bool clearAll = false);

		glm::vec3 GetBaseColor() const { return m_BaseColor; }
		float GetMetallic() const { return m_Metallic; }
		float GetRoughness() const { return m_Roughness; }
		void SetBaseColor(const glm::vec3& color) { m_BaseColor = color; }
		void SetMetallic(float metallic) { m_Metallic = metallic; }
		void SetRoughness(float roughness) { m_Roughness = roughness; }

	private:
		void BindTextures(Shader& shader);

		void BindFirstTextureByType(Shader& shader);
		void BindAndCountTexture(Shader& shader, int& textureUnit, MaterialTextureType type, const std::string& arrayUniform);
		void UploadTextureBlock(UniformBuffer& ubo, uint32_t entityIndex);

		int CountTexturesOfType(MaterialTextureType type) const;
		void CalculateTextureCounts();
		
		void SetupRawVertices(const std::vector<float>& rawVertices);
		void SetupVertex();
		void SetupVertexBufferLayout();
		void SetupElementBuffer();
		void SetupInstanceBuffer();

		std::vector<Vertex> m_Vertices;
		std::vector<unsigned int> m_Indices;
		std::vector<TextureData> m_Textures;

		glm::vec3 m_BaseColor = glm::vec3(1.0f);
		float m_Metallic = 0.0f;
		float m_Roughness = 1.0f;

		VertexBufferLayout m_Layout;
		VertexBufferLayout m_InstanceLayout;

		std::shared_ptr<VertexArray> m_VertexArray = nullptr;
		std::shared_ptr<VertexBuffer> m_VertexBuffer = nullptr;
		std::shared_ptr<VertexBuffer> m_InstanceVertexBuffer = nullptr;
		std::shared_ptr<ElementBuffer> m_ElementBuffer = nullptr;
		std::vector<glm::mat4> m_InstanceTransforms;

		bool m_HasEBO = false;
		bool m_HasInstancing = false;
		bool m_InstancingInitialized = false;

		std::unordered_map<MaterialTextureType, int> m_TextureCounts;

		GLenum m_DrawMode = GL_TRIANGLES;

		struct TextureBlockData
		{
			int NumDiffuse;
			int NumSpecular;
			int NumHeight;
			int NumNormal;
			int NumEmissive;
			int NumAO;
			int NumOpacity;
			int NumRoughness;
			int NumMetallic;

			glm::vec3 BaseColor;
			float pad0;		// padding para mantener std140
			float MetallicFactor;
			float RoughnessFactor;
			float pad1;		// padding para mantener std140
			bool UseGamma;	// en std140 ocupa 4 bytes
		};
	};
}