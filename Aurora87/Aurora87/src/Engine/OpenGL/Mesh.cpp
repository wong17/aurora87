#include "Mesh.h"

namespace Engine
{
	static void DebugMesh(const Mesh& mesh)
	{
		std::cout << "DebugMesh: Informacion de la malla:" << std::endl;

		// Información sobre los vértices
		std::cout << " - Numero de vertices: " << mesh.GetVertices().size() << std::endl;
		std::cout << " - Vertex Buffer ID: " << mesh.GetVertexBuffer()->GetRendererID() << std::endl;
		std::cout << " - Vertex Buffer Count: " << mesh.GetVertexBuffer()->GetVertexCount() << " vertices" << std::endl;
		std::cout << " - Vertex Buffer Size: " << mesh.GetVertexBuffer()->GetSize() << " bytes" << std::endl;

		// Información sobre el layout de los vértices
		const auto& layout = mesh.GetLayout();
		std::cout << " - Layout de los vertices:" << std::endl;
		for (const auto& element : layout.GetElements()) {
			std::cout << "     - Atributo: " << element.GetName() << std::endl;
			std::cout << "       - Tipo: " << static_cast<int>(element.GetType()) << std::endl;
			std::cout << "       - Normalizado: " << (element.GetNormalized() ? "Si" : "No") << std::endl;
			std::cout << "       - Offset: " << element.GetOffset() << std::endl;
			std::cout << "       - Componentes: " << element.GetComponentCount() << " elementos" << std::endl;
			std::cout << "       - Tamanio: " << element.GetSize() << " bytes" << std::endl;
		}

		// Información sobre los índices
		if (mesh.HasEBO()) {
			std::cout << " - Numero de indices: " << mesh.GetIndices().size() << std::endl;
			std::cout << " - Tiene Element Buffer Object (EBO)." << std::endl;
			std::cout << "   - Element Buffer ID: " << mesh.GetElementBuffer()->GetRendererID() << std::endl;
			std::cout << "   - Element Buffer Count: " << mesh.GetElementBuffer()->GetCount() << " elementos" << std::endl;
			std::cout << "   - Element Buffer Size: " << mesh.GetElementBuffer()->GetSize() << " bytes" << std::endl;
		}
		else {
			std::cout << " - No tiene Element Buffer Object (EBO)." << std::endl;
		}

		// Información sobre las texturas
		const auto& textures = mesh.GetTextures();
		std::cout << " - Numero de texturas: " << textures.size() << std::endl;
		for (size_t i = 0; i < textures.size(); ++i) {
			const auto& texture = textures[i];
			std::cout << "   Textura " << i + 1 << ":" << std::endl;
			std::cout << "     - Tipo: " << static_cast<int>(texture.Type) << std::endl;
			std::cout << "     - Texture ID: " << texture.Texture->GetRendererID() << std::endl;
			std::cout << "     - Uniform Name: " << texture.UniformName << std::endl;
			std::cout << "     - Texture Unit Index: " << texture.TextureUnitIndex << std::endl;
		}

		// Información sobre instancias
		if (mesh.HasInstancing()) {
			std::cout << " - Tiene instancias configuradas." << std::endl;
			std::cout << "   - Numero de transformaciones de instancia: " << mesh.GetInstanceTransforms().size() << std::endl;
			std::cout << "   - Layout de instancias:" << std::endl;
			const auto& instanceLayout = mesh.GetInstanceLayout();
			for (const auto& element : instanceLayout.GetElements()) {
				std::cout << "     - Atributo: " << element.GetName() << std::endl;
				std::cout << "       - Tipo: " << static_cast<int>(element.GetType()) << std::endl;
				std::cout << "       - Normalizado: " << (element.GetNormalized() ? "Si" : "No") << std::endl;
				std::cout << "       - Offset: " << element.GetOffset() << std::endl;
				std::cout << "       - Componentes: " << element.GetComponentCount() << " elementos" << std::endl;
				std::cout << "       - Tamanio: " << element.GetSize() << " bytes" << std::endl;
			}
		}
		else {
			std::cout << " - No tiene instancias configuradas." << std::endl;
		}

		// Información sobre el VertexArray
		std::cout << " - Vertex Array ID: " << mesh.GetVertexArray()->GetRendererID() << std::endl;
		std::cout << " - Vertex Buffer Index: " << mesh.GetVertexArray()->GetVertexBufferIndex() << std::endl;
	}

	Mesh::Mesh(
		const std::vector<float>& rawVertices,
		const VertexBufferLayout& layout,
		GLenum drawMode)
		: m_Indices(), m_Textures(), m_Layout(layout), m_DrawMode(drawMode), 
		m_BaseColor(glm::vec4(1.0f)), m_Metallic(0.0f), m_Roughness(1.0f)
	{
		SetupRawVertices(rawVertices);
		m_HasEBO = false;
		//DebugMesh(*this);
	}

	Mesh::Mesh(const std::vector<float>& rawVertices,
		const std::vector<TextureData>& textures,
		const VertexBufferLayout& layout,
		GLenum drawMode)
		: m_Indices(), m_Textures(textures), m_Layout(layout), m_DrawMode(drawMode), 
		m_BaseColor(glm::vec4(1.0f)), m_Metallic(0.0f), m_Roughness(1.0f)
	{
		CalculateTextureCounts();
		SetupRawVertices(rawVertices);
		m_HasEBO = false;
		//DebugMesh(*this);
	}

	Mesh::Mesh(
		const std::vector<float>& rawVertices,
		const std::vector<unsigned int>& indices, 
		const VertexBufferLayout& layout,
		GLenum drawMode)
		: m_Indices(indices), m_Textures(), m_Layout(layout), m_DrawMode(drawMode), 
		m_BaseColor(glm::vec4(1.0f)), m_Metallic(0.0f), m_Roughness(1.0f)
	{
		SetupRawVertices(rawVertices);
		m_HasEBO = !m_Indices.empty();
		SetupElementBuffer();
		//DebugMesh(*this);
	}

	Mesh::Mesh(
		const std::vector<Vertex>& vertices,
		const std::vector<TextureData>& textures,
		const VertexBufferLayout& layout,
		GLenum drawMode,
		glm::vec4 baseColor,
		float metallic,
		float roughness)
		: m_Vertices(vertices), m_Indices(), m_Textures(textures), m_Layout(layout),
		m_DrawMode(drawMode), m_BaseColor(baseColor), m_Metallic(metallic), m_Roughness(roughness)
	{
		CalculateTextureCounts();
		SetupVertex();
		//DebugMesh(*this);
	}

	Mesh::Mesh(
		const std::vector<Vertex>& vertices, 
		const std::vector<unsigned int>& indices,
		const std::vector<TextureData>& textures, 
		const VertexBufferLayout& layout,
		GLenum drawMode,
		glm::vec4 baseColor, 
		float metallic, 
		float roughness)
		: m_Vertices(vertices), m_Indices(indices), m_Textures(textures), m_Layout(layout),
		m_DrawMode(drawMode), m_BaseColor(baseColor), m_Metallic(metallic), m_Roughness(roughness)
	{
		CalculateTextureCounts();
		m_HasEBO = !m_Indices.empty();
		SetupVertex();
		//DebugMesh(*this);
	}

	void Mesh::SetInstanceTransforms(const std::vector<glm::mat4>& mats, const VertexBufferLayout& instanceLayout)
	{
		if (m_VertexArray == nullptr)
		{
			throw std::runtime_error("Mesh::SetInstanceTransforms(): VertexArray is null, use a constructor to create the Mesh.");
		}

		m_InstanceTransforms = mats;
		m_InstanceLayout = instanceLayout;
		m_HasInstancing = !m_InstanceTransforms.empty();
		SetupInstanceBuffer();
		//DebugMesh(*this);
	}

	void Mesh::DrawDepth(Shader& depthShader)
	{
		depthShader.Bind();

		if (m_VertexArray == nullptr) 
		{
			std::cerr << "Mesh::DrawDepth(): VertexArray is null." << std::endl;
			return;
		}

		m_VertexArray->Bind();
		if (m_HasEBO)
		{
			GLCall(glDrawElements(m_DrawMode, m_ElementBuffer->GetCount(), GL_UNSIGNED_INT, nullptr));
		}
		else
		{
			GLCall(glDrawArrays(m_DrawMode, 0, m_VertexBuffer->GetVertexCount()));
		}
	}

	void Mesh::DrawInstancedDepth(Shader& depthShader, uint32_t instanceCount)
	{
		if (!m_HasInstancing)
			throw std::runtime_error("Mesh::DrawInstancedDepth(): no instances have been configured (SetInstanceTransforms)");

		// Si no hay instancias registradas entonces se invoca Draw()
		if (instanceCount == 0) 
		{
			std::cout << "Mesh::DrawInstancedDepth(): instanceCount is 0, calling Draw()" << std::endl;
			DrawDepth(depthShader);
			return;
		}

		depthShader.Bind();

		if (m_VertexArray == nullptr) 
		{
			std::cerr << "Mesh::DrawInstancedDepth(): VertexArray is null." << std::endl;
			return;
		}

		m_VertexArray->Bind();
		if (m_HasEBO)
		{
			GLCall(glDrawElementsInstanced(m_DrawMode, m_ElementBuffer->GetCount(), GL_UNSIGNED_INT, nullptr, instanceCount));
		}
		else
		{
			GLCall(glDrawArraysInstanced(m_DrawMode, 0, m_VertexBuffer->GetVertexCount(), instanceCount));
		}
	}

	void Mesh::Draw(Shader& shader, UniformBuffer& textureBlockUniformBuffer, uint32_t entityIndex, bool bindTextures)
	{
		shader.Bind();

		UploadTextureBlock(textureBlockUniformBuffer, entityIndex);

		if (bindTextures)
		{
			//BindTextures(shader);
			BindFirstTextureByType(shader);
		}

		if (m_VertexArray == nullptr) 
		{
			std::cerr << "Mesh::Draw(): VertexArray is null." << std::endl;
			return;
		}

		m_VertexArray->Bind();
		if (m_HasEBO)
		{
			GLCall(glDrawElements(m_DrawMode, m_ElementBuffer->GetCount(), GL_UNSIGNED_INT, nullptr));
		}
		else
		{
			GLCall(glDrawArrays(m_DrawMode, 0, m_VertexBuffer->GetVertexCount()));
		}
	}

	void Mesh::DrawInstanced(Shader& shader, UniformBuffer& textureBlockUniformBuffer, uint32_t entityIndex, uint32_t instanceCount, bool bindTextures)
	{
		if (!m_HasInstancing)
			throw std::runtime_error("Mesh::DrawInstanced(): no instances have been configured (SetInstanceTransforms)");

		// Si no hay instancias registradas entonces se invoca Draw()
		if (instanceCount == 0) 
		{
			std::cout << "Mesh::DrawInstanced(): instanceCount is 0, calling Draw()" << std::endl;
			Draw(shader, textureBlockUniformBuffer, bindTextures);
			return;
		}

		UploadTextureBlock(textureBlockUniformBuffer, entityIndex);

		if (bindTextures)
		{
			//BindTextures(shader);
			BindFirstTextureByType(shader);
		}

		if (m_VertexArray == nullptr) 
		{
			std::cerr << "Mesh::DrawInstanced(): VertexArray is null." << std::endl;
			return;
		}

		m_VertexArray->Bind();
		if (m_HasEBO)
		{
			GLCall(glDrawElementsInstanced(m_DrawMode, m_ElementBuffer->GetCount(), GL_UNSIGNED_INT, nullptr, instanceCount));
		}
		else
		{
			GLCall(glDrawArraysInstanced(m_DrawMode, 0, m_VertexBuffer->GetVertexCount(), instanceCount));
		}
	}

	bool Mesh::CheckTextureLimit(MaterialTextureType type) const
	{
		int currentCount = CountTexturesOfType(type);
		return HasReachedTextureLimit(type, currentCount);
	}

	bool Mesh::NeedsGammaCorrection() const
	{
		for (auto& td : m_Textures)
			if (td.SRGB)
				return true;

		return false;
	}

	void Mesh::SetTextures(const std::vector<TextureData>& textures, bool clearAll)
	{
		if (clearAll) 
		{
			m_Textures.clear();
			m_TextureCounts.clear();
		}

		for (const auto& td : textures)
		{
			bool exists = std::any_of(m_Textures.begin(), m_Textures.end(), [&](const TextureData& existing) 
				{
					return existing.Type == td.Type	&& existing.Texture->GetRendererID() == td.Texture->GetRendererID();
				}
			);
			if (!exists)
			{
				m_Textures.push_back(td);
				m_TextureCounts[td.Type]++;
			}
		}
	}

	void Mesh::BindTextures(Shader& shader)
	{
		std::unordered_map<MaterialTextureType, int> typeCounters;
		int textureUnit = 0;
		for (auto& td : m_Textures) 
		{
			MaterialTextureType type = td.Type;
			int arrayIndex = typeCounters[type]++;
			std::string baseName = MaterialTextureUniformName(type);

			std::string uniformName = baseName;
			if (IsTextureTypeArray(type))
				uniformName += "[" + std::to_string(arrayIndex) + "]";

			td.Texture->Bind(td.TextureUnitIndex);
			shader.SetInt(uniformName, td.TextureUnitIndex);
			textureUnit++;
		}

		// gltf
		if (auto it = std::find_if(
			m_Textures.begin(), m_Textures.end(),
			[](auto const& td) { return td.Type == MaterialTextureType::BaseColor; });
			it != m_Textures.end())
		{
			it->Texture->Bind(textureUnit);
			shader.SetInt("u_AlbedoMap", textureUnit);
			textureUnit++;
		}
		if (auto it = std::find_if(
			m_Textures.begin(), m_Textures.end(),
			[](auto const& td) { return td.Type == MaterialTextureType::MetallicRoughness; });
			it != m_Textures.end())
		{
			it->Texture->Bind(textureUnit);
			shader.SetInt("u_MetallicRoughnessMap", textureUnit);
			textureUnit++;
		}
	}

	void Mesh::BindFirstTextureByType(Shader& shader)
	{
		std::unordered_map<MaterialTextureType, const TextureData*> firstOfType;
		for (const auto& td : m_Textures)
		{
			if (firstOfType.find(td.Type) == firstOfType.end())
				firstOfType[td.Type] = &td;
		}

		int textureUnit = 0;
		if (firstOfType.count(MaterialTextureType::Diffuse))
			BindAndCountTexture(shader, textureUnit, MaterialTextureType::Diffuse, "u_texture_diffuse");
		if (firstOfType.count(MaterialTextureType::Specular))
			BindAndCountTexture(shader, textureUnit, MaterialTextureType::Specular, "u_texture_specular");
		if (firstOfType.count(MaterialTextureType::Normal))
			BindAndCountTexture(shader, textureUnit, MaterialTextureType::Normal, "u_texture_normal");
		if (firstOfType.count(MaterialTextureType::Height))
			BindAndCountTexture(shader, textureUnit, MaterialTextureType::Height, "u_texture_height");
		if (firstOfType.count(MaterialTextureType::Emissive))
			BindAndCountTexture(shader, textureUnit, MaterialTextureType::Emissive, "u_texture_emissive");
		if (firstOfType.count(MaterialTextureType::AmbientOcclusion))
			BindAndCountTexture(shader, textureUnit, MaterialTextureType::AmbientOcclusion, "u_texture_ambient_occlusion");
		if (firstOfType.count(MaterialTextureType::Opacity))
			BindAndCountTexture(shader, textureUnit, MaterialTextureType::Opacity, "u_texture_opacity");
		if (firstOfType.count(MaterialTextureType::Roughness))
			BindAndCountTexture(shader, textureUnit, MaterialTextureType::Roughness, "u_texture_roughness");
		if (firstOfType.count(MaterialTextureType::Metallic))
			BindAndCountTexture(shader, textureUnit, MaterialTextureType::Metallic, "u_texture_metallic");

		// gltf
		if (firstOfType.count(MaterialTextureType::BaseColor))
		{
			auto td = firstOfType[MaterialTextureType::BaseColor];
			td->Texture->Bind(textureUnit);
			shader.SetInt("u_AlbedoMap", textureUnit);
			textureUnit++;
		}
		if (firstOfType.count(MaterialTextureType::MetallicRoughness))
		{
			auto td = firstOfType[MaterialTextureType::MetallicRoughness];
			td->Texture->Bind(textureUnit);
			shader.SetInt("u_MetallicRoughnessMap", textureUnit);
			textureUnit++;
		}
	}

	void Mesh::BindAndCountTexture(Shader& shader, int& textureUnit, MaterialTextureType type, const std::string& arrayUniform)
	{
		for (const auto& td : m_Textures)
		{
			if (td.Type == type)
			{
				td.Texture->Bind(textureUnit);
				shader.SetInt(arrayUniform + "[0]", textureUnit);
				textureUnit++;
				break;
			}
		}
	}

	void Mesh::UploadTextureBlock(UniformBuffer& ubo, uint32_t entityIndex)
	{
		uint32_t alignedStride = ubo.GetAlignedStride();
		uint32_t baseOffset = entityIndex * alignedStride;

		int numDiffuseTextures = (CountTexturesOfType(MaterialTextureType::Diffuse) > 0 ? 1 : 0);
		int numSpecularTextures = (CountTexturesOfType(MaterialTextureType::Specular) > 0 ? 1 : 0);
		int numHeightTextures = (CountTexturesOfType(MaterialTextureType::Height) > 0 ? 1 : 0);
		int numNormalTextures = (CountTexturesOfType(MaterialTextureType::Normal) > 0 ? 1 : 0);
		int numEmissiveTextures = (CountTexturesOfType(MaterialTextureType::Emissive) > 0 ? 1 : 0);
		int numAmbientOcclusionTextures = (CountTexturesOfType(MaterialTextureType::AmbientOcclusion) > 0 ? 1 : 0);
		int numOpacityTextures = (CountTexturesOfType(MaterialTextureType::Opacity) > 0 ? 1 : 0);
		int numRoughnessTextures = (CountTexturesOfType(MaterialTextureType::Roughness) > 0 ? 1 : 0);
		int numMetallicTextures = (CountTexturesOfType(MaterialTextureType::Metallic) > 0 ? 1 : 0);

		int hasAlbedoMap = (CountTexturesOfType(MaterialTextureType::BaseColor) > 0 ? 1 : 0);
		int hasMetallicRoughnessMap = (CountTexturesOfType(MaterialTextureType::MetallicRoughness) > 0 ? 1 : 0);

		glm::vec4 baseColor4 = m_BaseColor;
		float metallicFactor = m_Metallic;
		float roughnessFactor = m_Roughness;

		int useGamma = (NeedsGammaCorrection() ? 1 : 0);
		int useShadows = (g_EnableShadows.load() ? 1 : 0);

		std::unordered_map<std::string, std::pair<const void*, size_t>> valueMap = {
			{ "u_NumDiffuseTextures",            { &numDiffuseTextures,           sizeof(int) } },
			{ "u_NumSpecularTextures",           { &numSpecularTextures,          sizeof(int) } },
			{ "u_NumHeightTextures",             { &numHeightTextures,            sizeof(int) } },
			{ "u_NumNormalTextures",             { &numNormalTextures,            sizeof(int) } },
			{ "u_NumEmissiveTextures",           { &numEmissiveTextures,          sizeof(int) } },
			{ "u_NumAmbientOcclusionTextures",   { &numAmbientOcclusionTextures,  sizeof(int) } },
			{ "u_NumOpacityTextures",            { &numOpacityTextures,           sizeof(int) } },
			{ "u_NumRoughnessTextures",          { &numRoughnessTextures,         sizeof(int) } },
			{ "u_NumMetallicTextures",           { &numMetallicTextures,          sizeof(int) } },

			{ "u_HasAlbedoMap",                  { &hasAlbedoMap,                 sizeof(int) } },
			{ "u_HasMetallicRoughnessMap",       { &hasMetallicRoughnessMap,      sizeof(int) } },

			{ "u_BaseColor",                     { glm::value_ptr(baseColor4),    sizeof(glm::vec4) } },
			{ "u_MetallicFactor",                { &metallicFactor,               sizeof(float) } },
			{ "u_RoughnessFactor",               { &roughnessFactor,              sizeof(float) } },

			{ "u_UseGamma",                      { &useGamma,                     sizeof(int) } },
			{ "u_UseShadows",                    { &useShadows,                   sizeof(int) } },
		};

		ubo.Bind();
		const auto& layoutElems = ubo.GetLayout().GetElements();
		for (auto const& elem : layoutElems)
		{
			uint32_t offset = elem.Offset;
			uint32_t globalOffset = baseOffset + offset;
			const std::string& name = elem.Name;

			auto it = valueMap.find(name);
			if (it != valueMap.end())
			{
				const void* ptrData = it->second.first;
				uint32_t sizeInBytes = static_cast<uint32_t>(it->second.second);
				ubo.SetData(ptrData, sizeInBytes, globalOffset);
			}
		}

		ubo.BindRange(baseOffset, alignedStride);
	}

	int Mesh::CountTexturesOfType(MaterialTextureType type) const
	{
		auto it = m_TextureCounts.find(type);
		return (it != m_TextureCounts.end()) ? it->second : 0;
	}

	void Mesh::CalculateTextureCounts()
	{
		m_TextureCounts.clear();
		for (const auto& texture : m_Textures) 
		{
			m_TextureCounts[texture.Type]++;
		}
	}

	void Mesh::SetupRawVertices(const std::vector<float>& rawVertices)
	{
		m_VertexArray = std::make_shared<VertexArray>();
		m_VertexBuffer = std::make_shared<VertexBuffer>(rawVertices);
		m_VertexBuffer->SetLayout(m_Layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
	}

	void Mesh::SetupVertex()
	{
		// VertexArray
		m_VertexArray = std::make_shared<VertexArray>();

		// VertexBuffer
		uint32_t size = static_cast<uint32_t>(m_Vertices.size() * sizeof(Vertex));
		m_VertexBuffer = std::make_shared<VertexBuffer>(size);
		m_VertexBuffer->SetData(m_Vertices.data(), size);

		// To see if there is a mismatch between the buffer size and the number of vertices
		{
			uint32_t stride = m_Layout.GetStride();
			if (stride > 0)
			{
				uint32_t impliedCount = size / stride;
				uint32_t actualCount = static_cast<uint32_t>(m_Vertices.size());
				if (impliedCount != actualCount)
				{
					throw std::runtime_error(
						"Mesh::SetupMesh(): Vertex count mismatch warning. Buffer size = " +
						std::to_string(size) + " bytes, stride = " +
						std::to_string(stride) + " bytes -> " +
						std::to_string(impliedCount) + " vertices, but m_Vertices.size() = " +
						std::to_string(actualCount)
					);
				}
			}
		}

		// VertexBufferLayout
		SetupVertexBufferLayout();
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		// ElementBuffer
		SetupElementBuffer();
	}

	void Mesh::SetupVertexBufferLayout()
	{
		if (m_Layout.GetElements().empty())
			m_Layout = DefaultLayout();

		m_VertexBuffer->SetLayout(m_Layout);
	}

	void Mesh::SetupElementBuffer()
	{
		if (!m_HasEBO)
			return;

		m_ElementBuffer = std::make_shared<ElementBuffer>(m_Indices);
		m_VertexArray->SetElementBuffer(m_ElementBuffer);
	}

	void Mesh::SetupInstanceBuffer()
	{
		if (!m_HasInstancing)
			return;

		if (!m_InstancingInitialized)
		{
			m_InstanceVertexBuffer = std::make_shared<VertexBuffer>(m_InstanceTransforms);
			if (m_InstanceLayout.GetElements().empty())
				m_InstanceLayout = InstanceLayout();

			m_InstanceVertexBuffer->SetLayout(m_InstanceLayout);
			m_VertexArray->AddVertexBuffer(m_InstanceVertexBuffer, 1);

			m_InstancingInitialized = true;
			return;
		}

		// If it has already been initialized, we update the instance buffer
		m_InstanceVertexBuffer->Bind();
		m_InstanceVertexBuffer->SetData(m_InstanceTransforms.data(), 
			static_cast<uint32_t>(m_InstanceTransforms.size() * sizeof(glm::mat4)));
	}
}