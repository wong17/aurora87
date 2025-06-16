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

		// Mallas
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

		// Materiales y consumidor de texturas
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
				for (size_t i = 0; i < kv.second.size(); ++i) 
				{
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

		// Animaciones
		if (scene->mNumAnimations > 0)
		{
			std::cout << "Animaciones: " << scene->mNumAnimations << "\n";
			for (unsigned a = 0; a < scene->mNumAnimations; ++a)
			{
				const aiAnimation* anim = scene->mAnimations[a];
				std::string name = anim->mName.C_Str();
				if (name.empty()) name = "<sin nombre>";
				std::cout << "  [" << a << "] " << name
					<< " (duración: " << anim->mDuration << " ticks, "
					<< anim->mTicksPerSecond << " tps)\n";
			}
		}
		else
		{
			std::cout << "No hay animaciones en esta escena.\n";
		}
	}

	Model::Model(const std::string& path)
	{
		try {
			LoadModel(path);
			DebugModel(m_Scene);
			m_Valid = true;
		}
		catch (const std::exception& e) {
			std::cerr << "Model::Model: Model load failed: " << e.what() << std::endl;
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

		if (m_ModelAnimation) 
		{
			const auto& bones = m_ModelAnimation->GetBoneMatrices();
			depthShader.SetInt("u_HasAnimation", 1);
			depthShader.SetInt("u_NumBones", static_cast<int>(bones.size()));
			for (size_t i = 0; i < bones.size(); ++i) 
			{
				depthShader.SetMat4("u_Bones[" + std::to_string(i) + "]", bones[i]);
			}
		}
		else 
		{
			depthShader.SetInt("u_HasAnimation", 0);
		}

		for (auto& mesh : m_Meshes)
			mesh.DrawDepth(depthShader);
	}

	void Model::DrawInstancedDepth(Shader& depthShader, uint32_t instanceCount)
	{
		depthShader.Bind();

		if (m_ModelAnimation) 
		{
			const auto& bones = m_ModelAnimation->GetBoneMatrices();
			depthShader.SetInt("u_HasAnimation", 1);
			depthShader.SetInt("u_NumBones", static_cast<int>(bones.size()));
			for (size_t i = 0; i < bones.size(); ++i) 
			{
				depthShader.SetMat4("u_Bones[" + std::to_string(i) + "]", bones[i]);
			}
		}
		else 
		{
			depthShader.SetInt("u_HasAnimation", 0);
		}

		for (auto& mesh : m_Meshes)
			mesh.DrawInstancedDepth(depthShader, instanceCount);
	}

	void Model::Draw(
		Shader& shader,
		UniformBuffer& textureBlockUniformBuffer,
		uint32_t entityIndex,
		bool bindTextures)
	{
		shader.Bind();

		if (m_ModelAnimation) 
		{
			const auto& bones = m_ModelAnimation->GetBoneMatrices();
			shader.SetInt("u_HasAnimation", 1);
			shader.SetInt("u_NumBones", static_cast<int>(bones.size()));
			for (size_t i = 0; i < bones.size(); ++i) 
			{
				shader.SetMat4("u_Bones[" + std::to_string(i) + "]", bones[i]);
			}
		}
		else 
		{
			shader.SetInt("u_HasAnimation", 0);
		}

		for (auto& mesh : m_Meshes)
			mesh.Draw(shader, textureBlockUniformBuffer, entityIndex, bindTextures);
	}

	void Model::DrawInstanced(
		Shader& shader,
		UniformBuffer& textureBlockUniformBuffer,
		uint32_t entityIndex,
		uint32_t instanceCount, 
		bool bindTextures)
	{
		shader.Bind();

		if (m_ModelAnimation) 
		{
			const auto& bones = m_ModelAnimation->GetBoneMatrices();
			shader.SetInt("u_HasAnimation", 1);
			shader.SetInt("u_NumBones", static_cast<int>(bones.size()));
			for (size_t i = 0; i < bones.size(); ++i) 
			{
				shader.SetMat4("u_Bones[" + std::to_string(i) + "]", bones[i]);
			}
		}
		else 
		{
			shader.SetInt("u_HasAnimation", 0);
		}

		for (auto& mesh : m_Meshes)
			mesh.DrawInstanced(shader, textureBlockUniformBuffer, entityIndex, instanceCount, bindTextures);
	}

	size_t Model::GetAnimationCount() const
	{
		return m_ModelAnimation ? m_ModelAnimation->GetAnimationCount() : 0;
	}

	const std::vector<std::string>& Model::GetAnimationNames() const
	{
		static const std::vector<std::string> empty;
		return m_ModelAnimation	? m_ModelAnimation->GetAnimationNames() : empty;
	}

	bool Model::SetAnimation(size_t idx)
	{
		if (!m_ModelAnimation) 
		{
			std::cerr << "Model::SetAnimation: no animations loaded.\n";
			return false;
		}
		size_t count = m_ModelAnimation->GetAnimationCount();
		if (idx >= count) 
		{
			std::cerr << "Model::SetAnimation: index out of range ("	<< idx << " >= " << count << ").\n";
			return false;
		}

		return m_ModelAnimation->SetAnimation(idx);
	}

	bool Model::SetAnimation(const std::string& name)
	{
		if (!m_ModelAnimation) 
		{
			std::cerr << "Model::SetAnimation: no animations loaded.\n";
			return false;
		}

		if (name.empty()) 
		{
			std::cerr << "Model::SetAnimation: animation name is empty.\n";
			return false;
		}
		const auto& names = m_ModelAnimation->GetAnimationNames();
		if (std::find(names.begin(), names.end(), name) == names.end()) 
		{
			std::cerr << "Model::SetAnimation: animation \"" << name << "\" not found.\n";
			return false;
		}

		return m_ModelAnimation->SetAnimation(name);
	}

	void Model::UpdateAnimation(float deltaTime) 
	{
		if (!m_ModelAnimation) 
		{
			return;
		}

		if (deltaTime < 0.0f) 
		{
			std::cerr << "Model::UpdateAnimation: negative deltaTime (" << deltaTime << ").\n";
			return;
		}

		m_ModelAnimation->Update(deltaTime);
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
		// Detect extension to decide FlipUVs
		m_Extension = std::filesystem::path(path).extension().string();
		std::transform(m_Extension.begin(), m_Extension.end(), m_Extension.begin(), ::tolower);
		m_Directory = std::filesystem::path(path).parent_path().string();

		// Base Flags
		unsigned int flags =
			aiProcess_Triangulate
			| aiProcess_JoinIdenticalVertices
			| aiProcess_ValidateDataStructure
			;

		// PBR Formats (glTF/GLB)
		if (m_Extension == ".gltf" || m_Extension == ".glb") 
		{
			flags |= aiProcess_EmbedTextures
				| aiProcess_CalcTangentSpace   // Normal + tangent
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
		// Others
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

		if (m_Scene->mNumAnimations > 0) 
		{
			m_ModelAnimation.emplace(m_Scene);
			std::cout << "Model::LoadModel: loaded " << m_ModelAnimation->GetAnimationCount()
				<< " animations\n";
		}

		uint32_t nextTexUnit = 0;
		ProcessNode(m_Scene->mRootNode, m_Scene, nextTexUnit);
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene, uint32_t& nextTexUnit)
	{
		// For each mesh at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.emplace_back(ProcessMesh(mesh, scene, nextTexUnit));
		}
		// Recursively process each child of the node
		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			ProcessNode(node->mChildren[i], scene, nextTexUnit);
		}
	}

	Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, uint32_t& nextTexUnit)
	{
		// 1. Vertices (Reserve space for vertices)
		auto vertices = ExtractVertices(mesh);
		// 2. Indexes (Reserve space for face indexes)
		auto indices = ExtractIndices(mesh);
		// 3. Textures (Extract textures)
		auto textures = ExtractTextures(mesh, scene, nextTexUnit);
		// 4) PBR factors
		glm::vec4 baseColor;
		float metallic, roughness;
		ExtractPBRFactors(mesh, scene, baseColor, metallic, roughness);
		// 5) Drawing mode
		GLenum drawMode = DetermineDrawMode(mesh);

		return Mesh(vertices, indices, textures, Mesh::DefaultModelLayout(), drawMode, baseColor, metallic, roughness);
	}

	std::vector<Vertex> Model::ExtractVertices(aiMesh* mesh) const
	{
		// 1. Vertices (Reserve space for vertices)
		std::vector<Vertex> vertices;
		vertices.reserve(mesh->mNumVertices);
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex{};
			// Position
			vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };

			// Normal
			vertex.Normal = mesh->HasNormals() ?
				glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z) :
				glm::vec3(0.0f);

			// Texture coordinates
			if (mesh->mTextureCoords[0])
				vertex.TexCoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };

			// Tangents and bitangents
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
		// Get texture list for current extension
		auto it = TEXTURE_TYPES_BY_EXTENSION.find(m_Extension);
		if (it == TEXTURE_TYPES_BY_EXTENSION.end()) {
			std::cerr << "Model::ExtractTextures: Unsupported format: " << m_Extension << "\n";
			return textures;
		}
		// Cargar texturas de cada tipo
		const auto& baseTypes = it->second;
		for (auto t : baseTypes) 
		{
			auto maps = LoadMaterialTextures(mat, t, nextTexUnit);
			textures.insert(textures.end(), maps.begin(), maps.end());
		}

		return textures;
	}

	std::vector<TextureData> Model::LoadMaterialTextures(aiMaterial* mat, MaterialTextureType mtt, uint32_t& nextTextureUnit)
	{
		std::vector<TextureData> result;
		// Collect all texture paths
		auto paths = CollectTexturePaths(mat, mtt);
		result.reserve(paths.size());

		for (size_t i = 0; i < paths.size(); ++i)
		{
			bool embedded = false;
			int  embeddedIndex = -1;
			// From aiString, determine whether embedded or file, return clean pathId
			auto optFs = ResolveTexturePath(paths[i], embedded, embeddedIndex);
			std::string pathId = embeddedIndex >= 0 ? "__embedded_" + std::to_string(embeddedIndex) : (optFs ? *optFs : paths[i].C_Str());
			// See if it has already been searched
			if (auto it = m_LoadedTexturesMap.find(pathId); it != m_LoadedTexturesMap.end())
			{
				result.push_back(it->second);
				continue;
			}

			// Load and create TextureData, or std::nullopt on error
			if (auto td = CreateTextureData(pathId, mtt, nextTextureUnit))
			{
				result.push_back(*td);
				m_LoadedTexturesMap.emplace(pathId, *td);
			}
		}

		return result;
	}

	std::vector<aiString> Model::CollectTexturePaths(aiMaterial* mat, MaterialTextureType mtt) const
	{
		std::vector<aiString> paths;

		aiString path;
		aiTextureType aType = MaterialTextureAssimpType(mtt);
		unsigned count = mat->GetTextureCount(aType);
		paths.reserve(count);

		for (unsigned i = 0; i < count; ++i) 
		{
			if (mat->GetTexture(aType, i, &path) == AI_SUCCESS) 
				paths.push_back(path);
		}

		if (paths.empty() && (m_Extension == ".glb" || m_Extension == ".gltf")) 
		{
			switch (mtt) 
			{
				case MaterialTextureType::BaseColor:
					aType = aiTextureType_DIFFUSE;
					break;
				case MaterialTextureType::MetallicRoughness:
					aType = aiTextureType_SPECULAR;
					break;
				default:
					return paths;
			}

			count = mat->GetTextureCount(aType);
			for (unsigned i = 0; i < count; ++i) 
			{
				if (mat->GetTexture(aType, i, &path) == AI_SUCCESS)
				{
					paths.push_back(path);
				}
			}
		}

		return paths;
	}

	// Common texture subdirectories
	static const std::vector<std::string> textureSubdirs =
	{
		"textures", "texture",       // Most common
		"images", "image",
		"materials", "material",
		"source", "sources",
		"tex", "diffuse", "normal",
		"Textures", "Images",
		"Materials", "Sources"
	};

	std::optional<std::string> Model::ResolveTexturePath(const aiString& aiStr, bool& outEmbedded, int& outEmbeddedIndex) const
	{
		std::string s = aiStr.C_Str();

		// Embedded ("*0", "*1", …)
		if (!s.empty() && s.front() == '*')
		{
			try 
			{
				outEmbeddedIndex = std::stoi(s.substr(1));
				outEmbedded = true;
			}
			catch (const std::exception& e) 
			{
				std::cerr << "Model::ResolveTexturePath: Indice embebido invalido: " << s << " | Error: " << e.what() << "\n";
				outEmbedded = false;
				outEmbeddedIndex = -1;
			}
			return std::nullopt;
		}
		outEmbedded = false;
		outEmbeddedIndex = -1;

		// Normalize separators and remove quotation marks if any
		std::replace(s.begin(), s.end(), '\\', '/');
		s.erase(std::remove(s.begin(), s.end(), '\"'), s.end());
		std::filesystem::path texPath = std::filesystem::path(s).lexically_normal();

		const std::vector<std::filesystem::path> baseDirs = 
		{
			m_Directory,
			std::filesystem::path(m_Directory).parent_path()
		};

		auto checkPath = [](const std::filesystem::path& p) -> std::optional<std::string> 
		{
			if (std::filesystem::exists(p)) 
				return p.lexically_normal().string();

			return std::nullopt;
		};

		// Texture search
		for (const auto& base : baseDirs) 
		{
			// Direct routes
			std::filesystem::path candidates[] = 
			{
				base / texPath,
				base / texPath.filename()
			};

			for (const auto& c : candidates) 
			{
				if (auto path = checkPath(c)) 
				{
					return path;
				}

				// Common Subdirectories
				for (const auto& sub : textureSubdirs) 
				{
					if (auto path = checkPath(base / sub / texPath)) 
					{
						return path;
					}
				}
			}
		}

		// Error with tested routes
		std::cerr << "Model::ResolveTexturePath: Texture not found: " << texPath << "\n";
		
		return std::nullopt;
	}

	std::shared_ptr<Texture> Model::LoadTextureFromPath(const std::string& pathId, bool embedded, int embeddedIndex, const TextureSpecification& spec) const
	{
		if (embedded)
		{
			// glTF embedded texture
			aiTexture* tex = m_Scene->mTextures[embeddedIndex];
			return AssimpTextureLoader::LoadEmbeddedTexture(tex, spec);
		}
		// external texture
		if (!std::filesystem::exists(pathId))
		{
			std::cerr << "Model::LoadTextureFromPath: does not exist " << pathId << "\n";
			return nullptr;
		}

		return TextureManager::Get().Load(pathId, spec);
	}

	std::optional<TextureData> Model::CreateTextureData(const std::string& pathId, MaterialTextureType mtt, uint32_t& nextTextureUnit) const
	{
		// intentamos cargar
		bool embedded = (pathId.rfind("__embedded_", 0) == 0);
		int embeddedIndex = embedded ? std::stoi(pathId.substr(std::string("__embedded_").length())) : -1;

		if (embedded) 
		{
			if (embeddedIndex < 0 || embeddedIndex >= static_cast<int>(m_Scene->mNumTextures))
			{
				std::cerr << "Model::CreateTextureData: Embedded index invalid: " << embeddedIndex << "\n";
				return std::nullopt;
			}
		}

		TextureSpecification spec;
		spec.SRGB = TextureTypeIsSRGB(mtt);

		// Load Texture (embedded or file)
		auto tex = LoadTextureFromPath(pathId, embedded, embeddedIndex, spec);
		if (!tex)
		{
			std::cerr << "Model::CreateTextureData: load failure " << pathId << "\n";
			return std::nullopt;
		}

		tex->SetName(pathId);
		std::string uniformName = MaterialTextureUniformName(mtt);
		TextureData td(mtt, tex, uniformName, nextTextureUnit++, spec.SRGB, pathId);

		return td;
	}

	void Model::ExtractPBRFactors(aiMesh* mesh, const aiScene* scene, glm::vec4& outBaseColor, float& outMetallic, float& outRoughness) const
	{
		outBaseColor = glm::vec4(1.0f);
		outMetallic = 0.0f;
		outRoughness = 1.0f;

		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		if (m_Extension == ".gltf" || m_Extension == ".glb") 
		{
			aiColor4D baseColor(1.0f);
			if (mat->Get(AI_MATKEY_BASE_COLOR, baseColor) == AI_SUCCESS) 
			{
				outBaseColor = glm::vec4(baseColor.r, baseColor.g, baseColor.b, baseColor.a);
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
			aiColor4D diffuse(1.0f);
			if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse) == AI_SUCCESS) 
			{
				outBaseColor = glm::vec4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
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
				// Fallback to shininess if no roughness
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
			aiColor4D diffuse(1.0f);
			if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse))
			{
				outBaseColor = glm::vec4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
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

		// always triangulated by aiProcess_Triangulate:
		return GL_TRIANGLES;
	}
}