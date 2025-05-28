#pragma once

#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include "TextureManager.h"
#include "AssimpTextureLoader.h"
#include "ModelAnimation.h"

#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <assimp/GltfMaterial.h>
#include <assimp/texture.h>
#include <assimp/version.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <filesystem>
#include <cstdint>
#include <algorithm>
#include <unordered_map>
#include <stdexcept>
#include <optional>
#include <functional>

namespace Engine
{
	class Model
	{
	public:
		explicit Model(const std::string& path);
		~Model() = default;

		Model(const Model&) = delete;
		Model& operator=(const Model&) = delete;
		Model(Model&&) = default;
		Model& operator=(Model&&) = default;

		bool IsValid() const { return m_Valid; }

		void SetInstanceTransforms(const std::vector<glm::mat4>& mats, const VertexBufferLayout& instanceLayout = Mesh::InstanceLayout());
		void DrawDepth(Shader& depthShader);
		void DrawInstancedDepth(Shader& depthShader, uint32_t instanceCount = 0);
		void Draw(Shader& shader, bool bindTextures = true);
		void DrawInstanced(Shader& shader, uint32_t instanceCount = 0, bool bindTextures = true);

		size_t GetAnimationCount() const;
		const std::vector<std::string>& GetAnimationNames() const;
		bool SetAnimation(size_t idx);
		bool SetAnimation(const std::string& name);

		void UpdateAnimation(float deltaTime);

		inline const std::string& GetExtension() const { return m_Extension; }
		inline const std::string& GetDirectory() const { return m_Directory; }
		inline const std::vector<Mesh>& GetMeshes() { return m_Meshes; }
		inline const std::unordered_map<std::string, TextureData>& GetLoadedTextures() { return m_LoadedTexturesMap; }

		bool NeedsGammaCorrection() const;

	private:
		void LoadModel(const std::string& path);
		void ProcessNode(aiNode* node, const aiScene* scene, uint32_t& nextTexUnit);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, uint32_t& nextTexUnit);
		std::vector<TextureData> LoadMaterialTextures(aiMaterial* mat, MaterialTextureType mtt, uint32_t& nextTextureUnit);
		// Funciones para procesar la malla
		std::vector<Vertex> ExtractVertices(aiMesh* mesh) const;
		std::vector<unsigned int> ExtractIndices(aiMesh* mesh) const;
		std::vector<TextureData> ExtractTextures(aiMesh* mesh, const aiScene* scene, uint32_t& nextTexUnit);
		void ExtractPBRFactors(aiMesh* mesh, const aiScene* scene, glm::vec3& outBaseColor, float& outMetallic, float& outRoughness) const;
		GLenum DetermineDrawMode(aiMesh* mesh) const;
		// Funciones para cargar texturas
		std::vector<aiString> CollectTexturePaths(aiMaterial* mat, MaterialTextureType mtt) const;
		std::optional<std::string> ResolveTexturePath(const aiString& aiStr, bool& outEmbedded, int& outEmbeddedIndex) const;
		std::shared_ptr<Texture> LoadTextureFromPath(const std::string& pathId, bool embedded, int embeddedIndex, const TextureSpecification& spec) const;
		std::optional<TextureData> CreateTextureData(const std::string& pathId, MaterialTextureType mtt, uint32_t& nextTextureUnit) const;

		std::string m_Extension;
		std::string m_Directory;
		Assimp::Importer m_Importer;										// Debe vivir mientras usemos aiScene
		const aiScene* m_Scene = nullptr;
		std::vector<Mesh> m_Meshes;
		std::unordered_map<std::string, TextureData> m_LoadedTexturesMap;	// Para evitar cargas duplicadas, las cacheamos
		bool m_Valid = false;

		std::optional<ModelAnimation> m_ModelAnimation;
	};
}