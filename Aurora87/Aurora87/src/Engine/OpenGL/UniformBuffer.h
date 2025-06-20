#pragma once

#include "UniformBufferLayout.h"
#include "UniformBufferElement.h"
#include "Shader.h"

#include "Engine/Core/DebugGL.h"

#include "Engine/Scene/GlobalSceneSettings.h"

#include <glad/glad.h>

#include <cstdint>
#include <string>
#include <stdexcept>
#include <iostream>
#include <initializer_list>

namespace Engine
{
	class UniformBuffer
	{
	public:
		// Default value for blockCount (number of entities)
		static constexpr uint32_t DEFAULT_BLOCK_COUNT = 100;

		UniformBuffer(uint32_t bindingPoint = 0)
			: UniformBuffer(GetDefaultLayout(), bindingPoint, DEFAULT_BLOCK_COUNT) { }
		UniformBuffer(const UniformBufferLayout& layout, uint32_t bindingPoint)
			: UniformBuffer(layout, bindingPoint, DEFAULT_BLOCK_COUNT) { }
		UniformBuffer(const UniformBufferLayout& layout, uint32_t bindingPoint, uint32_t blockCount);
		~UniformBuffer();

		void Bind() const;
		void Unbind() const;
		void BindRange(uint32_t offset, uint32_t size) const;
		void SetData(const std::string& name, const void* data) const;
		void SetData(const void* data, uint32_t size, uint32_t offset = 0) const;
		void BindToShader(const Shader& shader, const std::string& blockName, uint32_t bindingPoint);
		void BindToShader(const std::string& blockName,	uint32_t bindingPoint, 
			std::initializer_list<std::reference_wrapper<const Shader>> shaders);
		void BindToShader(const std::string& blockName,
			std::initializer_list<std::reference_wrapper<const Shader>> shaders);

		uint32_t GetRendererID() const { return m_RendererID; }
		uint32_t GetBindingPoint() const { return m_BindingPoint; }
		const UniformBufferLayout& GetLayout() const { return m_Layout; }

		// Adjusts the number of blocks (number of entities) and re-allocates the buffer.
		void ResizeBlocks(uint32_t blockCount);
		uint32_t GetBlockCount() const { return m_BlockCount; }
		uint32_t GetStride() const { return m_Stride; }
		uint32_t GetAlignedStride() const { return m_AlignedStride; }

		static UniformBufferLayout GetDefaultLayout() 
		{
			return 
			{
				{ ShaderDataType::Mat4, "u_ModelMatrix" },
				{ ShaderDataType::Mat4, "u_ViewMatrix" },
				{ ShaderDataType::Mat4, "u_ProjectionMatrix" }
			};
		}
		static UniformBufferLayout GetCameraLayout()
		{
			return
			{
				{ ShaderDataType::Mat4, "u_ViewMatrix" },
				{ ShaderDataType::Mat4, "u_ProjectionMatrix" }
			};
		}
		static UniformBufferLayout GetModelLayout()
		{
			return
			{
				{ ShaderDataType::Mat4, "u_ModelMatrix" }
			};
		}
		static UniformBufferLayout GetTextureBlockLayout()
		{
			return
			{
				{ ShaderDataType::Int,    "u_NumDiffuseTextures" },
				{ ShaderDataType::Int,    "u_NumSpecularTextures" },
				{ ShaderDataType::Int,    "u_NumHeightTextures" },
				{ ShaderDataType::Int,    "u_NumNormalTextures" },
				{ ShaderDataType::Int,    "u_NumEmissiveTextures" },
				{ ShaderDataType::Int,    "u_NumAmbientOcclusionTextures" },
				{ ShaderDataType::Int,    "u_NumOpacityTextures" },
				{ ShaderDataType::Int,    "u_NumRoughnessTextures" },
				{ ShaderDataType::Int,    "u_NumMetallicTextures" },

				{ ShaderDataType::Int,    "u_HasAlbedoMap" },
				{ ShaderDataType::Int,    "u_HasMetallicRoughnessMap" },

				{ ShaderDataType::Float4, "u_BaseColor" },
				{ ShaderDataType::Float,  "u_MetallicFactor" },
				{ ShaderDataType::Float,  "u_RoughnessFactor" },

				{ ShaderDataType::Int,   "u_UseGamma" },
				{ ShaderDataType::Int,   "u_UseShadows" }
			};
		}
		static UniformBufferLayout GetLightBlockLayout(int maxDirLights, int maxPointLights, int maxSpotLights)
		{
			std::vector<UniformBufferElement> elements;

			elements.push_back({ ShaderDataType::Int, "u_NumDirectionalLights" });
			elements.push_back({ ShaderDataType::Int, "u_NumPointLights" });
			elements.push_back({ ShaderDataType::Int, "u_NumSpotLights" });

			elements.push_back({ ShaderDataType::Float4, "u_GlobalAmbient" });

			for (int i = 0; i < maxDirLights; i++)
			{
				elements.push_back({ ShaderDataType::Float4, "directionalLights[" + std::to_string(i) + "].direction" });
				elements.push_back({ ShaderDataType::Float4, "directionalLights[" + std::to_string(i) + "].ambient" });
				elements.push_back({ ShaderDataType::Float4, "directionalLights[" + std::to_string(i) + "].diffuse" });
				elements.push_back({ ShaderDataType::Float4, "directionalLights[" + std::to_string(i) + "].specular" });
			}

			for (int i = 0; i < maxPointLights; i++)
			{
				elements.push_back({ ShaderDataType::Float4, "pointLights[" + std::to_string(i) + "].position" });
				elements.push_back({ ShaderDataType::Float,  "pointLights[" + std::to_string(i) + "].constant" });
				elements.push_back({ ShaderDataType::Float,  "pointLights[" + std::to_string(i) + "].linear" });
				elements.push_back({ ShaderDataType::Float,  "pointLights[" + std::to_string(i) + "].quadratic" });
				elements.push_back({ ShaderDataType::Float4, "pointLights[" + std::to_string(i) + "].ambient" });
				elements.push_back({ ShaderDataType::Float4, "pointLights[" + std::to_string(i) + "].diffuse" });
				elements.push_back({ ShaderDataType::Float4, "pointLights[" + std::to_string(i) + "].specular" });
			}

			for (int i = 0; i < maxSpotLights; i++)
			{
				elements.push_back({ ShaderDataType::Float4, "spotLights[" + std::to_string(i) + "].position" });
				elements.push_back({ ShaderDataType::Float4, "spotLights[" + std::to_string(i) + "].direction" });
				elements.push_back({ ShaderDataType::Float,  "spotLights[" + std::to_string(i) + "].cutOff" });
				elements.push_back({ ShaderDataType::Float,  "spotLights[" + std::to_string(i) + "].outerCutOff" });
				elements.push_back({ ShaderDataType::Float,  "spotLights[" + std::to_string(i) + "].constant" });
				elements.push_back({ ShaderDataType::Float,  "spotLights[" + std::to_string(i) + "].linear" });
				elements.push_back({ ShaderDataType::Float,  "spotLights[" + std::to_string(i) + "].quadratic" });
				elements.push_back({ ShaderDataType::Float4, "spotLights[" + std::to_string(i) + "].ambient" });
				elements.push_back({ ShaderDataType::Float4, "spotLights[" + std::to_string(i) + "].diffuse" });
				elements.push_back({ ShaderDataType::Float4, "spotLights[" + std::to_string(i) + "].specular" });
			}

			return UniformBufferLayout(elements);
		}

	private:
		uint32_t m_RendererID;
		uint32_t m_BindingPoint;
		UniformBufferLayout m_Layout;

		uint32_t m_Stride;				// stride without padding
		uint32_t m_AlignedStride;		// stride with padding
		uint32_t m_BlockCount;
	};
}