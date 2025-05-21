#include "Model.h"

namespace Engine
{
	static void DebugModel(const aiScene* scene)
	{
		if (!scene) {
			std::cerr << "DebugModel: No se ha cargado ninguna escena." << std::endl;
			return;
		}

		std::cout << "DebugModel: Información del modelo cargado:" << std::endl;

		// Información general de la escena
		std::cout << " - Numero de mallas: " << scene->mNumMeshes << std::endl;
		std::cout << " - Numero de materiales: " << scene->mNumMaterials << std::endl;
		std::cout << " - Numero de texturas embebidas: " << scene->mNumTextures << std::endl;

		// Iterar sobre las mallas
		for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
			const aiMesh* mesh = scene->mMeshes[i];
			std::cout << "   Malla " << i + 1 << ":" << std::endl;
			std::cout << "     - Nombre: " << (mesh->mName.length > 0 ? mesh->mName.C_Str() : "Sin nombre") << std::endl;
			std::cout << "     - Numero de vertices: " << mesh->mNumVertices << std::endl;
			std::cout << "     - Numero de caras: " << mesh->mNumFaces << std::endl;
			std::cout << "     - Tiene normales: " << (mesh->HasNormals() ? "Si" : "No") << std::endl;
			std::cout << "     - Tiene tangentes y bitangentes: " << (mesh->HasTangentsAndBitangents() ? "Si" : "No") << std::endl;
			std::cout << "     - Numero de canales UV: " << mesh->GetNumUVChannels() << std::endl;
			std::cout << "     - Numero de canales de color: " << mesh->GetNumColorChannels() << std::endl;
			std::cout << "     - Material index: " << mesh->mMaterialIndex << std::endl;
			std::cout << "     - Tipo de malla: " << ((mesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE) ? "Triangulo" : "Desconocido") << std::endl;
			std::cout << "     - Tamanio de la malla: " << mesh->mNumVertices * sizeof(aiVector3D) << " bytes" << std::endl;
			std::cout << "     - Tamanio de los indices: " << mesh->mNumFaces * sizeof(aiFace) << " bytes" << std::endl;
			std::cout << "     - Tamanio de las texturas: " << mesh->mNumUVComponents[0] * sizeof(aiVector3D) << " bytes" << std::endl;
		}

		// Iterar sobre los materiales
		for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
			const aiMaterial* material = scene->mMaterials[i];
			aiString name;
			material->Get(AI_MATKEY_NAME, name);
			std::cout << "   Material " << i + 1 << ":" << std::endl;
			std::cout << "     - Nombre: " << name.C_Str() << std::endl;
			std::cout << "     - Numero de texturas difusas: " << material->GetTextureCount(aiTextureType_DIFFUSE) << std::endl;
			std::cout << "     - Numero de texturas especulares: " << material->GetTextureCount(aiTextureType_SPECULAR) << std::endl;
			std::cout << "     - Numero de texturas normales: " << material->GetTextureCount(aiTextureType_NORMALS) << std::endl;
			std::cout << "     - Numero de texturas de altura: " << material->GetTextureCount(aiTextureType_HEIGHT) << std::endl;
			std::cout << "     - Numero de texturas de emisividad: " << material->GetTextureCount(aiTextureType_EMISSIVE) << std::endl;
			std::cout << "     - Numero de texturas de oclusión: " << material->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION) << std::endl;
			std::cout << "     - Numero de texturas de opacidad: " << material->GetTextureCount(aiTextureType_OPACITY) << std::endl;
			std::cout << "     - Numero de texturas de metalicidad: " << material->GetTextureCount(aiTextureType_METALNESS) << std::endl;
			std::cout << "     - Numero de texturas de rugosidad: " << material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS) << std::endl;
			std::cout << "     - Numero de texturas de base color: " << material->GetTextureCount(aiTextureType_BASE_COLOR) << std::endl;
		}

		// Texturas embebidas
		if (scene->mNumTextures > 0) {
			std::cout << " - Texturas embebidas:" << std::endl;
			for (unsigned int i = 0; i < scene->mNumTextures; ++i) {
				const aiTexture* texture = scene->mTextures[i];
				std::cout << "   Textura " << i + 1 << ":" << std::endl;
				std::cout << "     - Ancho: " << texture->mWidth << std::endl;
				std::cout << "     - Altura: " << (texture->mHeight > 0 ? std::to_string(texture->mHeight) : "Compresión DDS") << std::endl;
				std::cout << "     - Formato: " << (texture->achFormatHint[0] != '\0' ? texture->achFormatHint : "Desconocido") << std::endl;
			}
		}
	}

	Model::Model(const std::string& path)
	{
		try {
			LoadModel(path);
			//DebugModel(m_Scene);
			m_Valid = true;
		}
		catch (const std::exception& e) {
			std::cerr << "Model load failed: " << e.what() << std::endl;
			m_Valid = false;
		}
	}

	void Model::SetInstanceTransforms(const std::vector<glm::mat4>& mats, const VertexBufferLayout& instanceLayout)
	{
		for (auto& mesh : m_Meshes)
		{
			mesh.SetInstanceTransforms(mats, instanceLayout);
		}
	}

	void Model::DrawDepth(Shader& depthShader)
	{
		depthShader.Bind();
		for (auto& mesh : m_Meshes)
			mesh.DrawDepth(depthShader);
	}

	void Model::DrawInstancedDepth(Shader& depthShader, uint32_t instanceCount)
	{
		depthShader.Bind();
		for (auto& mesh : m_Meshes)
			mesh.DrawInstancedDepth(depthShader, instanceCount);
	}

	void Model::Draw(Shader& shader, bool bindTextures)
	{
		shader.Bind();
		for (auto& mesh : m_Meshes)
			mesh.Draw(shader, bindTextures);
	}

	void Model::DrawInstanced(Shader& shader, uint32_t instanceCount, bool bindTextures)
	{
		shader.Bind();
		for (auto& mesh : m_Meshes)
			mesh.DrawInstanced(shader, instanceCount, bindTextures);
	}

	bool Model::NeedsGammaCorrection() const
	{
		for (auto& mesh : m_Meshes)
			if (mesh.NeedsGammaCorrection())
				return true;

		return false;
	}

	void Model::LoadModel(const std::string& path)
	{
		// Detectar extensión para decidir FlipUVs
		m_Extension = std::filesystem::path(path).extension().string();
		std::transform(m_Extension.begin(), m_Extension.end(), m_Extension.begin(), ::tolower);
		m_Directory = std::filesystem::path(path).parent_path().string();

		unsigned int flags = 
			aiProcess_Triangulate | aiProcess_CalcTangentSpace | 
			aiProcess_JoinIdenticalVertices | aiProcess_OptimizeMeshes;

		if (m_Extension != ".gltf" && m_Extension != ".glb") {
			flags |= aiProcess_FlipUVs;
		}

		m_Scene = m_Importer.ReadFile(path, flags);
		if (!m_Scene || m_Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_Scene->mRootNode) {
			std::cerr << "Model::LoadModel: " << m_Importer.GetErrorString() << std::endl;
			return;
		}

		uint32_t nextTexUnit = 0;
		ProcessNode(m_Scene->mRootNode, m_Scene, nextTexUnit);
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene, uint32_t& nextTexUnit)
	{
		// Para cada malla en el nodo actual
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.emplace_back(ProcessMesh(mesh, scene, nextTexUnit));
		}
		// Recursivamente procesa cada hijo del nodo
		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			ProcessNode(node->mChildren[i], scene, nextTexUnit);
		}
	}

	Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, uint32_t& nextTexUnit)
	{
		// 1. Vertices (Reservar espacio para los vértices)
		auto vertices = ExtractVertices(mesh);
		// 2. Indices (Reservar espacio para los índices de las caras)
		auto indices = ExtractIndices(mesh);
		// 3. Texturas (Extraer las texturas)
		auto textures = ExtractTextures(mesh, scene, nextTexUnit);
		// 4) Factores PBR
		glm::vec3 baseColor;
		float metallic, roughness;
		ExtractPBRFactors(mesh, scene, baseColor, metallic, roughness);
		// 5) Modo de dibujo
		GLenum drawMode = DetermineDrawMode(mesh);

		return Mesh(vertices, indices, textures, Mesh::DefaultModelLayout(), drawMode, baseColor, metallic, roughness);
	}

	std::vector<TextureData> Model::LoadMaterialTextures(aiMaterial* mat, MaterialTextureType mtt, uint32_t& nextTextureUnit)
	{
		std::vector<TextureData> result;
		// Recolectar todas las rutas de la textura
		auto paths = CollectTexturePaths(mat, mtt);
		result.reserve(paths.size());

		for (size_t i = 0; i < paths.size(); ++i)
		{
			bool embedded = false;
			int  embeddedIndex = -1;
			// A partir de aiString, determinar si es embedded o archivo, devolver pathId limpio
			std::string pathId = ResolveTexturePath(paths[i], embedded, embeddedIndex);

			// Ver si ya la cacheamos
			if (auto it = m_LoadedTexturesMap.find(pathId); it != m_LoadedTexturesMap.end())
			{
				result.push_back(it->second);
				continue;
			}

			// Cargar y crear un TextureData, o std::nullopt en error
			if (auto td = CreateTextureData(pathId, mtt, nextTextureUnit))
			{
				result.push_back(*td);
				m_LoadedTexturesMap.emplace(pathId, *td);
			}
		}

		return result;
	}
	
	std::vector<Vertex> Model::ExtractVertices(aiMesh* mesh) const
	{
		// 1. Vertices (Reservar espacio para los vértices)
		std::vector<Vertex> vertices;
		vertices.reserve(mesh->mNumVertices);
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex{};
			// Posición
			vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };

			// Normales
			vertex.Normal = mesh->HasNormals() ?
				glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z) :
				glm::vec3(0.0f);

			// Coordenadas de textura
			if (mesh->mTextureCoords[0])
				vertex.TexCoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };

			// Tangentes y bitangentes
			if (mesh->mTangents && mesh->mBitangents) {
				vertex.Tangent = { mesh->mTangents[i].x,   mesh->mTangents[i].y,   mesh->mTangents[i].z };
				vertex.Bitangent = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
			}

			vertices.push_back(vertex);
		}

		return vertices;
	}

	std::vector<unsigned int> Model::ExtractIndices(aiMesh* mesh) const
	{
		std::vector<unsigned int> indices;
		indices.reserve(static_cast<std::vector<unsigned int, std::allocator<unsigned int>>::size_type>(mesh->mNumFaces) * 3u);
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			const aiFace& face = mesh->mFaces[i];
			indices.insert(indices.end(), face.mIndices, face.mIndices + face.mNumIndices);
		}

		return indices;
	}

	std::vector<TextureData> Model::ExtractTextures(aiMesh* mesh, const aiScene* scene, uint32_t& nextTexUnit)
	{
		std::vector<TextureData> textures;
		if (mesh->mMaterialIndex < 0) return textures;

		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		// Obtener lista de texturas para la extensión actual
		auto it = TEXTURE_TYPES_BY_EXTENSION.find(m_Extension);
		if (it == TEXTURE_TYPES_BY_EXTENSION.end()) {
			std::cerr << "Formato no soportado: " << m_Extension << "\n";
			return textures;
		}
		// Cargar texturas de cada tipo
		const auto& baseTypes = it->second;
		for (auto t : baseTypes) {
			auto maps = LoadMaterialTextures(mat, t, nextTexUnit);
			textures.insert(textures.end(), maps.begin(), maps.end());
		}

		return textures;
	}

	void Model::ExtractPBRFactors(aiMesh* mesh, const aiScene* scene, glm::vec3& outBaseColor, float& outMetallic, float& outRoughness) const
	{
		outBaseColor = glm::vec3(1.0f);
		outMetallic = 0.0f;
		outRoughness = 1.0f;

		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		if (m_Extension == ".gltf" || m_Extension == ".glb")
		{
			aiColor3D baseColor(1, 1, 1);
			if (AI_SUCCESS == mat->Get(AI_MATKEY_BASE_COLOR, baseColor)) {
				outBaseColor = glm::vec3(baseColor.r, baseColor.g, baseColor.b);
			}
			mat->Get(AI_MATKEY_METALLIC_FACTOR, outMetallic);
			mat->Get(AI_MATKEY_ROUGHNESS_FACTOR, outRoughness);
		}
		else if (m_Extension == ".fbx")
		{
			// en FBX normalmente hay un color difuso y un "shininess"
			aiColor3D diffuse(1, 1, 1);
			if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse)) {
				outBaseColor = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
			}

			if (AI_SUCCESS != mat->Get(AI_MATKEY_METALLIC_FACTOR, outMetallic)) {
				// Fallback a conversión de shininess
				// shininess ∈ [0..max] -> lo convertimos a roughness = 1 - (shininess / maxShininess)
				float shininess = 100.0f;
				if (AI_SUCCESS == mat->Get(AI_MATKEY_SHININESS, shininess)) {
					const float maxShine = 1000.0f;  // Valor más realista para FBX
					outRoughness = 1.0f - glm::clamp(shininess / maxShine, 0.0f, 1.0f);
				}
			}
		}
		else if (m_Extension == ".obj")
		{
			aiColor3D diffuse(1.0f, 1.0f, 1.0f);
			if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse)) {
				outBaseColor = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
			}
		}
		else
		{
			std::cerr << "Model::ExtractPBRFactors: Formato no implementado para PBR: " << m_Extension << "\n";
		}
	}

	GLenum Model::DetermineDrawMode(aiMesh* mesh) const
	{
		auto prim = mesh->mPrimitiveTypes;
		if (prim & aiPrimitiveType_POINT)    return GL_POINTS;
		if (prim & aiPrimitiveType_LINE)     return GL_LINES;

		// siempre triangulado por aiProcess_Triangulate:
		return GL_TRIANGLES;
	}

	std::vector<aiString> Model::CollectTexturePaths(aiMaterial* mat, MaterialTextureType mtt) const
	{
		std::vector<aiString> paths;
		aiTextureType aType = MaterialTextureAssimpType(mtt);

		if (mtt == MaterialTextureType::MetallicRoughness)
		{
			aiString s;
			if (mat->GetTexture(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLICROUGHNESS_TEXTURE, &s) == AI_SUCCESS)
				paths.push_back(s);
		}
		else
		{
			unsigned count = mat->GetTextureCount(aType);
			paths.reserve(count);
			for (unsigned i = 0; i < count; ++i)
			{
				aiString s;
				if (mat->GetTexture(aType, i, &s) == AI_SUCCESS)
					paths.push_back(s);
			}
		}

		return paths;
	}

	std::string Model::ResolveTexturePath(const aiString& aiStr, bool& outEmbedded, int& outEmbeddedIndex) const
	{
		std::string id = aiStr.C_Str();
		outEmbedded = false;
		outEmbeddedIndex = -1;

		if (!id.empty() && id[0] == '*')
		{
			// formato "*N"
			outEmbeddedIndex = std::stoi(id.substr(1));
			outEmbedded = true;
			return "__embedded_" + std::to_string(outEmbeddedIndex);
		}
		// ruta de fichero
		return (std::filesystem::path(m_Directory) / id).string();
	}

	std::shared_ptr<Texture> Model::LoadTextureFromPath(const std::string& pathId, bool embedded, int embeddedIndex, const TextureSpecification& spec) const
	{
		if (embedded)
		{
			// textura embebida glTF
			aiTexture* tex = m_Scene->mTextures[embeddedIndex];
			return AssimpTextureLoader::LoadEmbeddedTexture(tex, spec);
		}
		// textura externa
		if (!std::filesystem::exists(pathId))
		{
			std::cerr << "Model::LoadTextureFromPath: no existe " << pathId << "\n";
			return nullptr;
		}

		return TextureManager::Get().Load(pathId, spec);
	}

	std::optional<TextureData> Model::CreateTextureData(const std::string& pathId, MaterialTextureType mtt, uint32_t& nextTextureUnit) const
	{
		// intentamos cargar
		bool embedded = (pathId.rfind("__embedded_", 0) == 0);
		int embeddedIndex = embedded
			? std::stoi(pathId.substr(std::string("__embedded_").length()))
			: -1;
		
		TextureSpecification spec;
		spec.SRGB = TextureTypeIsSRGB(mtt);

		// Cargar la Texture (embebido o de fichero)
		auto tex = LoadTextureFromPath(pathId, embedded, embeddedIndex, spec);
		if (!tex)
		{
			std::cerr << "Model::LoadMaterialTextures: fallo carga " << pathId << "\n";
			return std::nullopt;
		}

		std::string uniformName = MaterialTextureUniformName(mtt);
		TextureData td(mtt, tex, uniformName, nextTextureUnit++, spec.SRGB, pathId);

		return td;
	}
}