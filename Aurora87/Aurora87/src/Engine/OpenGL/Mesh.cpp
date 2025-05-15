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

	struct TexInfo { int unit, kind; };

	Mesh::Mesh(
		const std::vector<float>& rawVertices,
		const VertexBufferLayout& layout,
		GLenum drawMode)
		: m_Indices(), m_Textures(), m_Layout(layout), m_DrawMode(drawMode), 
		m_BaseColor(glm::vec3(1.0f)), m_Metallic(0.0f), m_Roughness(1.0f)
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
		m_BaseColor(glm::vec3(1.0f)), m_Metallic(0.0f), m_Roughness(1.0f)
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
		m_BaseColor(glm::vec3(1.0f)), m_Metallic(0.0f), m_Roughness(1.0f)
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
		glm::vec3 baseColor,
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
		glm::vec3 baseColor, 
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
			throw std::runtime_error("Mesh::SetInstanceTransforms(): VertexArray es null, utilice un constructor para crear el Mesh.");
		}

		m_InstanceTransforms = mats;
		m_InstanceLayout = instanceLayout;
		m_HasInstancing = !m_InstanceTransforms.empty();
		SetupInstanceBuffer();
		//DebugMesh(*this);
	}

	void Mesh::Draw(Shader& shader, bool bindTextures)
	{
		shader.Bind();
		shader.SetInt("u_UseGamma", NeedsGammaCorrection());
		shader.SetFloat3("u_BaseColor", m_BaseColor);
		shader.SetFloat("u_MetallicFactor", m_Metallic);
		shader.SetFloat("u_RoughnessFactor", m_Roughness);

		if (bindTextures)
			BindTextures(shader);

		if (m_VertexArray == nullptr) {
			std::cerr << "Mesh::Draw(): VertexArray es null." << std::endl;
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

	void Mesh::DrawInstanced(Shader& shader, uint32_t instanceCount, bool bindTextures)
	{
		if (!m_HasInstancing)
			throw std::runtime_error("Mesh::DrawInstanced(): no se han configurado instancias (SetInstanceTransforms)");

		// Si no hay instancias registradas entonces se invoca Draw()
		if (instanceCount == 0) {
			std::cout << "Mesh::DrawInstanced(): instanceCount es 0, llamando a Draw()" << std::endl;
			Draw(shader, bindTextures);
			return;
		}

		shader.Bind();
		shader.SetInt("u_UseGamma", NeedsGammaCorrection());
		shader.SetFloat3("u_BaseColor", m_BaseColor);
		shader.SetFloat("u_MetallicFactor", m_Metallic);
		shader.SetFloat("u_RoughnessFactor", m_Roughness);

		if (bindTextures)
			BindTextures(shader);

		if (m_VertexArray == nullptr) {
			std::cerr << "Mesh::DrawInstanced(): VertexArray es null." << std::endl;
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

	void Mesh::BindTextures(Shader& shader)
	{
		//// 1) Bind de cada textura a su unidad
		//for (auto& td : m_Textures)
		//	td.Texture->Bind(td.TextureUnitIndex);

		//// 2) Preparamos el array de TexInfo
		//std::vector<TexInfo> infos;
		//infos.reserve(m_Textures.size());
		//for (auto& td : m_Textures) 
		//{
		//	infos.push_back({ int(td.TextureUnitIndex), int(td.Type) });
		//}

		//// 3) Subir u_NumTexInfo
		//shader.SetInt("u_NumTexInfo", int(infos.size()));

		//// 4) Para cada TexInfo[i], subimos sus campos
		//for (int i = 0; i < (int)infos.size(); ++i) {
		//	shader.SetInt("u_TexInfo[" + std::to_string(i) + "].unit", infos[i].unit);
		//	shader.SetInt("u_TexInfo[" + std::to_string(i) + "].kind", infos[i].kind);
		//}

		//// 5) Finalmente, nuestros samplers u_Textures[0..MAX_TEX_INFO-1] deben apuntar
		//// a las unidades de textura 0..MAX_TEX_INFO-1.  
		//// Sólo necesitamos hacer esto UNA vez, podría hacerse en OnAttach():
		//static int units[MAX_TEX_INFO];
		//static bool samplersInitialized = false;
		//if (!samplersInitialized) {
		//	for (int j = 0; j < MAX_TEX_INFO; j++)
		//		units[j] = j;
		//	shader.SetIntArray("u_Textures", units, MAX_TEX_INFO);
		//	samplersInitialized = true;
		//}

		std::unordered_map<MaterialTextureType, int> typeCounters;
		for (auto& td : m_Textures) {
			std::string uniformName = MaterialTextureUniformName(td.Type);

			if (!IsTextureTypeArray(td.Type))
			{
				if (typeCounters[td.Type]++ > 0)
				{
					std::cerr << "Mesh::BindTextures: Error multiples texturas para tipo unico " << static_cast<int>(td.Type) << "\n";
					continue;
				}
			}
			else
			{
				int arrayIndex = typeCounters[td.Type]++;
				uniformName += "[" + std::to_string(arrayIndex) + "]";
			}

			td.Texture->Bind(td.TextureUnitIndex);
			shader.SetInt(uniformName, td.TextureUnitIndex);
		}

		shader.SetInt("u_NumDiffuseTextures", CountTexturesOfType(MaterialTextureType::Diffuse));
		shader.SetInt("u_NumSpecularTextures", CountTexturesOfType(MaterialTextureType::Specular));
		shader.SetInt("u_NumHeightTextures", CountTexturesOfType(MaterialTextureType::Height));
		shader.SetInt("u_NumNormalTextures", CountTexturesOfType(MaterialTextureType::Normal));
		shader.SetInt("u_NumEmissiveTextures", CountTexturesOfType(MaterialTextureType::Emissive));
		shader.SetInt("u_NumAmbientOcclusionTextures", CountTexturesOfType(MaterialTextureType::AmbientOcclusion));
		shader.SetInt("u_NumOpacityTextures", CountTexturesOfType(MaterialTextureType::Opacity));
		shader.SetInt("u_NumRoughnessTextures", CountTexturesOfType(MaterialTextureType::Roughness));
		shader.SetInt("u_NumMetallicTextures", CountTexturesOfType(MaterialTextureType::Metallic));
	}

	int Mesh::CountTexturesOfType(MaterialTextureType type) const
	{
		auto it = m_TextureCounts.find(type);
		return (it != m_TextureCounts.end()) ? it->second : 0;
	}

	void Mesh::CalculateTextureCounts()
	{
		m_TextureCounts.clear();
		for (const auto& texture : m_Textures) {
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

		// Para ver si hay un desajuste entre el tamaño del buffer y el número de vértices
		{
			uint32_t stride = m_Layout.GetStride();
			if (stride > 0)
			{
				uint32_t impliedCount = size / stride;
				uint32_t actualCount = static_cast<uint32_t>(m_Vertices.size());
				if (impliedCount != actualCount)
				{
					throw std::runtime_error(
						"Mesh::SetupMesh(): Advertencia desajuste en el conteo de vertices. Tamanio del buffer = " +
						std::to_string(size) + " bytes, stride = " +
						std::to_string(stride) + " bytes -> " +
						std::to_string(impliedCount) + " vertices, pero m_Vertices.size() = " +
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

		m_InstanceVertexBuffer = std::make_shared<VertexBuffer>(m_InstanceTransforms);
		if (m_InstanceLayout.GetElements().empty())
			m_InstanceLayout = InstanceLayout();

		m_InstanceVertexBuffer->SetLayout(m_InstanceLayout);
		m_VertexArray->AddVertexBuffer(m_InstanceVertexBuffer, 1);
	}
}