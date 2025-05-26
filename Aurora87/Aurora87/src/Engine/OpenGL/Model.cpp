#include "Model.h"

namespace Engine
{
	static void DebugModel(const aiScene* scene)
	{
		if (!scene)
		{
			std::cerr << "DebugModel: No se ha cargado ninguna escena.\n";
			return;
		}

		// Cabecera
		std::cout << "Assimp " << aiGetVersionMajor() << "."
			<< aiGetVersionMinor() << "."
			<< aiGetVersionPatch() << "\n"
			<< "DebugModel: #" << scene->mNumMeshes << " meshes, "
			<< scene->mNumMaterials << " materials, "
			<< scene->mNumTextures << " embedded textures\n";

		// --- MALLAS ---
		for (unsigned i = 0; i < scene->mNumMeshes; ++i)
		{
			const aiMesh* m = scene->mMeshes[i];
			struct { const char* label; std::function<std::string()> val; } props[] = {
				{ "Name",       [&] { return m->mName.length ? m->mName.C_Str() : "Sin nombre"; } },
				{ "Vertices",   [&] { return std::to_string(m->mNumVertices); } },
				{ "Faces",      [&] { return std::to_string(m->mNumFaces); } },
				{ "Normals",    [&] { return m->HasNormals() ? "Si" : "No"; } },
				{ "Tangents",   [&] { return m->HasTangentsAndBitangents() ? "Si" : "No"; } },
				{ "UV channels",[&] { return std::to_string(m->GetNumUVChannels()); } },
				{ "Color ch.",  [&] { return std::to_string(m->GetNumColorChannels()); } },
				{ "Mat idx",    [&] { return std::to_string(m->mMaterialIndex); } },
				{ "Mode",       [&] { return (m->mPrimitiveTypes & aiPrimitiveType_TRIANGLE) ?
										   "Triangulo" : "Otro"; } },
				{ "Mesh size",  [&] { return std::to_string(m->mNumVertices * sizeof(aiVector3D)) + " bytes"; } },
				{ "Index size", [&] { return std::to_string(m->mNumFaces * sizeof(aiFace)) + " bytes"; } }
			};
			std::cout << " Malla " << (i + 1) << ":\n";
			for (auto& p : props)
				std::cout << "  - " << std::setw(12) << p.label << ": " << p.val() << "\n";
		}

		// --- MATERIALES y consumidor de texturas ---
		struct TexType { aiTextureType t; const char* name; };
		static constexpr TexType texTypes[] = {
			{ aiTextureType_DIFFUSE,           "Diffuse" },
			{ aiTextureType_SPECULAR,          "Specular" },
			{ aiTextureType_NORMALS,           "Normal" },
			{ aiTextureType_HEIGHT,            "Height" },
			{ aiTextureType_EMISSIVE,          "Emissive" },
			{ aiTextureType_AMBIENT_OCCLUSION, "AO" },
			{ aiTextureType_OPACITY,           "Opacity" },
			{ aiTextureType_METALNESS,         "Metallic" },
			{ aiTextureType_DIFFUSE_ROUGHNESS, "Roughness" },
			{ aiTextureType_BASE_COLOR,        "BaseColor" },
			{ aiTextureType_GLTF_METALLIC_ROUGHNESS,"M-R" }
		};

		// Mapeo de uso de embeddings
		std::unordered_map<int, std::vector<std::string>> usage;
		for (unsigned m = 0; m < scene->mNumMaterials; ++m)
		{
			aiString matName; scene->mMaterials[m]->Get(AI_MATKEY_NAME, matName);
			std::cout << " Material " << (m + 1) << ": " << matName.C_Str() << "\n";
			for (auto& tt : texTypes)
			{
				unsigned cnt = scene->mMaterials[m]->GetTextureCount(tt.t);
				if (!cnt) continue;
				std::cout << "  * " << tt.name << ": " << cnt << "\n";
				for (unsigned i = 0; i < cnt; ++i) {
					aiString path;
					if (scene->mMaterials[m]->GetTexture(tt.t, i, &path) == AI_SUCCESS
						&& path.C_Str()[0] == '*')
					{
						int idx = std::stoi(path.C_Str() + 1);
						usage[idx].push_back(tt.name);
					}
				}
			}
		}

		// Uso de embeddings
		if (!usage.empty())
		{
			std::cout << " Uso de textures embebidas:\n";
			for (auto& kv : usage)
			{
				std::cout << "  - Tex[" << (kv.first + 1) << "]: ";
				for (size_t i = 0; i < kv.second.size(); ++i) {
					std::cout << kv.second[i] << (i + 1 < kv.second.size() ? ", " : "");
				}
				std::cout << "\n";
			}
		}

		// Raw embedded textures
		for (unsigned i = 0; i < scene->mNumTextures; ++i)
		{
			const aiTexture* T = scene->mTextures[i];
			std::cout << " Raw Tex " << (i + 1)
				<< "  WxH: " << T->mWidth << " x "
				<< (T->mHeight > 0 ? std::to_string(T->mHeight) : "DDS")
				<< ", fmt: " << (T->achFormatHint[0] ? T->achFormatHint : "??")
				<< "\n";
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

		// Flags base
		unsigned int flags =
			aiProcess_Triangulate
			| aiProcess_JoinIdenticalVertices
			| aiProcess_ValidateDataStructure
			;

		// Formatos PBR (glTF/GLB)
		if (m_Extension == ".gltf" || m_Extension == ".glb")
		{
			flags |= aiProcess_EmbedTextures
				| aiProcess_CalcTangentSpace   // Normales + tangentes
				| aiProcess_ImproveCacheLocality
				| aiProcess_RemoveRedundantMaterials;
		}
		// FBX
		else if (m_Extension == ".fbx")
		{
			flags |= aiProcess_FlipUVs
				| aiProcess_CalcTangentSpace
				| aiProcess_LimitBoneWeights;
		}
		// OBJ
		else if (m_Extension == ".obj")
		{
			flags |= aiProcess_FlipUVs
				| aiProcess_CalcTangentSpace
				| aiProcess_OptimizeGraph
				| aiProcess_SortByPType;
		}
		// Otros
		else
		{
			flags |= aiProcess_FlipUVs
				| aiProcess_CalcTangentSpace
				| aiProcess_OptimizeMeshes;
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
			if (mat->Get(AI_MATKEY_BASE_COLOR, baseColor) == AI_SUCCESS)
			{
				outBaseColor = glm::vec3(baseColor.r, baseColor.g, baseColor.b);
			}

			ai_real metallic = 0.0f;
			if (mat->Get(AI_MATKEY_METALLIC_FACTOR, metallic) == AI_SUCCESS)
			{
				outMetallic = static_cast<float>(metallic);
			}

			ai_real roughness = 1.0f;
			if (mat->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness) == AI_SUCCESS)
			{
				outRoughness = static_cast<float>(roughness);
			}
		}
		else if (m_Extension == ".fbx")
		{
			aiColor3D diffuse(1, 1, 1);
			if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse) == AI_SUCCESS)
			{
				outBaseColor = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
			}

			// Metallic
			ai_real metallic = 0.0f;
			if (mat->Get(AI_MATKEY_METALLIC_FACTOR, metallic) == AI_SUCCESS)
			{
				outMetallic = static_cast<float>(metallic);
			}

			// Roughness
			ai_real roughness = 1.0f;
			if (mat->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness) != AI_SUCCESS)
			{
				// Fallback a shininess si no hay roughness
				ai_real shininess = 100.0f;
				if (mat->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
				{
					const float maxShine = 500.0f;
					outRoughness = 1.0f - glm::clamp(static_cast<float>(shininess) / maxShine, 0.0f, 1.0f);
				}
			}
			else
			{
				outRoughness = static_cast<float>(roughness);
			}
		}
		else if (m_Extension == ".obj")
		{
			aiColor3D diffuse(1.0f, 1.0f, 1.0f);
			if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse))
			{
				outBaseColor = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
			}

			outMetallic = 0.0f;
			outRoughness = 0.8f;
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