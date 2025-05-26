#pragma once

#include "Engine/OpenGL/Texture.h"

#include <assimp/scene.h>
#include <assimp/texture.h>
#include <assimp/GltfMaterial.h>
#include <assimp/material.h>
#include <glm/glm.hpp>

#include <string>
#include <cstdint>
#include <vector>
#include <unordered_map>

namespace Engine
{
    constexpr int MAX_TEXTURES_PER_TYPE = 7;

	// Enum para los tipos de texturas
    enum class MaterialTextureType
    {
        // Texturas en arreglo
        Diffuse,
        Specular,
        Normal,
        Height,
        Emissive,
        AmbientOcclusion,
        Opacity,
        Roughness,
        Metallic,

		// Texturas específicas para glTF/GLB
		MetallicRoughness,  // u_MetallicRoughnessMap
		BaseColor,          // u_AlbedoMap

        None,
		Unknown
    };

    inline bool IsTextureTypeArray(MaterialTextureType type) {
        switch (type) {
            case MaterialTextureType::BaseColor:
            case MaterialTextureType::MetallicRoughness:
                return false;
            default:
                return true;
        }
    }

    inline bool HasReachedTextureLimit(MaterialTextureType type, int currentCount) {
        if (!IsTextureTypeArray(type)) {
            // Para texturas unicas, maximo 1
            return currentCount >= 1;
        }
        return currentCount >= MAX_TEXTURES_PER_TYPE;
    }

	// Para aplicar gamma correction solo a las texturas que lo requieran
    inline bool TextureTypeIsSRGB(MaterialTextureType t) {
		return t == MaterialTextureType::Diffuse    // .fbx/.obj
            || t == MaterialTextureType::BaseColor; // .glTF/.glb albedo
    }


	// Para convertir de MaterialTextureType a aiTextureType y usarlo en el shader
    inline std::string MaterialTextureUniformName(MaterialTextureType type)
    {
        switch (type) 
        {
            case MaterialTextureType::Diffuse:              return "u_texture_diffuse";
            case MaterialTextureType::Specular:             return "u_texture_specular";
            case MaterialTextureType::Normal:               return "u_texture_normal";
            case MaterialTextureType::Height:               return "u_texture_height";
            case MaterialTextureType::Emissive:             return "u_texture_emissive";
            case MaterialTextureType::AmbientOcclusion:     return "u_texture_ambient_occlusion";
            case MaterialTextureType::Opacity:              return "u_texture_opacity";
            case MaterialTextureType::BaseColor:            return "u_AlbedoMap";
            case MaterialTextureType::MetallicRoughness:    return "u_MetallicRoughnessMap";
            case MaterialTextureType::Unknown:              return "u_texture_unknown";
            case MaterialTextureType::Roughness:            return "u_texture_roughness";
            case MaterialTextureType::Metallic:             return "u_texture_metallic";
            default:                                        return "";
        }

        return "";
    }

	// Para convertir de MaterialTextureType a aiTextureType y usarlo al cargar texturas
    inline aiTextureType MaterialTextureAssimpType(MaterialTextureType type)
    {
        switch (type)
        {
            case MaterialTextureType::Diffuse:              return aiTextureType_DIFFUSE;
            case MaterialTextureType::Specular:             return aiTextureType_SPECULAR;
            case MaterialTextureType::Normal:               return aiTextureType_NORMALS;
            case MaterialTextureType::Height:               return aiTextureType_HEIGHT;
            case MaterialTextureType::Emissive:             return aiTextureType_EMISSIVE;
		    case MaterialTextureType::AmbientOcclusion:     return aiTextureType_AMBIENT_OCCLUSION;
		    case MaterialTextureType::Opacity:              return aiTextureType_OPACITY;
            case MaterialTextureType::BaseColor:            return aiTextureType_BASE_COLOR;
            case MaterialTextureType::MetallicRoughness:    return aiTextureType_GLTF_METALLIC_ROUGHNESS;
		    case MaterialTextureType::Unknown:              return aiTextureType_UNKNOWN;
            case MaterialTextureType::Roughness:            return aiTextureType_DIFFUSE_ROUGHNESS;
            case MaterialTextureType::Metallic:             return aiTextureType_METALNESS;
            case MaterialTextureType::None:
            default:                                        return aiTextureType_NONE;
        }
        return aiTextureType_NONE;
    }

    const std::unordered_map<std::string, std::vector<MaterialTextureType>> TEXTURE_TYPES_BY_EXTENSION = {
        // Formatos PBR (glTF/GLB)
        {".gltf", {
            MaterialTextureType::BaseColor,         // glTF albedo
            MaterialTextureType::MetallicRoughness,
            MaterialTextureType::Normal,
            MaterialTextureType::AmbientOcclusion,
            MaterialTextureType::Emissive,
        }},
        {".glb", {
			MaterialTextureType::BaseColor,         // glTF albedo
            MaterialTextureType::MetallicRoughness,
            MaterialTextureType::Normal,
            MaterialTextureType::AmbientOcclusion,
            MaterialTextureType::Emissive
        }},

        // Formatos tradicionales (FBX)
        {".fbx", {
            MaterialTextureType::Diffuse,
            MaterialTextureType::Specular,
            MaterialTextureType::Normal,
            MaterialTextureType::Height,
            MaterialTextureType::Emissive,
            MaterialTextureType::AmbientOcclusion,
            MaterialTextureType::Opacity,
            MaterialTextureType::Roughness,
            MaterialTextureType::Metallic
        }},

        // Formatos simples (OBJ)
        {".obj", {
            MaterialTextureType::Diffuse,
            MaterialTextureType::Specular,
            MaterialTextureType::Normal
        }}
    };

    struct Vertex
    {
        Vertex() = default;
		
        Vertex(
            const glm::vec3& position)
            : Position(position), Normal(glm::vec3(0.0f)), TexCoord(glm::vec2(0.0f)), Tangent(glm::vec3(0.0f)), Bitangent(glm::vec3(0.0f)) {}
		
        Vertex(
            const glm::vec3& position, 
            const glm::vec2& texCoord)
			: Position(position), Normal(glm::vec3(0.0f)), TexCoord(texCoord), Tangent(glm::vec3(0.0f)), Bitangent(glm::vec3(0.0f)) {}
		
        Vertex(
            const glm::vec3& position, 
            const glm::vec3& normal, 
            const glm::vec2& texCoord)
			: Position(position), Normal(normal), TexCoord(texCoord), Tangent(glm::vec3(0.0f)), Bitangent(glm::vec3(0.0f)) {}
        
        Vertex(
            const glm::vec3& position, 
            const glm::vec3& normal, 
            const glm::vec2& texCoord, 
            const glm::vec3& tangent, 
            const glm::vec3& bitangent)
            : Position(position), Normal(normal), TexCoord(texCoord), Tangent(tangent), Bitangent(bitangent) {}

        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoord;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
    };

    struct TextureData
    {
        TextureData() : Type(MaterialTextureType::None), UniformName(""), TextureUnitIndex(0) {}

        TextureData(
            MaterialTextureType type, 
            std::shared_ptr<Engine::Texture> texture, 
            std::string uName, 
            uint32_t unit)
            : Type(type), Texture(texture), UniformName(uName), TextureUnitIndex(unit) {}

		TextureData(
            MaterialTextureType type, 
            std::shared_ptr<Engine::Texture> texture, 
            std::string uName, 
            uint32_t unit, 
            std::string cacheKey)
            : Type(type), Texture(texture), UniformName(uName), TextureUnitIndex(unit), CacheKey(cacheKey) {}
        
        TextureData(
            MaterialTextureType type,
            std::shared_ptr<Engine::Texture> texture,
            std::string uName,
            uint32_t unit,
            bool srgb,
            std::string cacheKey = "")
            : Type(type), Texture(texture), UniformName(uName), TextureUnitIndex(unit), SRGB(srgb), CacheKey(cacheKey) {}

        bool SRGB = false;
        std::shared_ptr<Engine::Texture> Texture;
        MaterialTextureType Type;
        std::string UniformName;
        uint32_t TextureUnitIndex;
        std::string CacheKey;
    };
}