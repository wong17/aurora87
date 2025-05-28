#pragma once

#include "Animation.h"
#include "Animator.h"
#include "Bone.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/scene.h>

#include <vector>
#include <string>
#include <map>
#include <optional>
#include <cstdint>
#include <iostream>
#include <stdexcept>

namespace Engine
{
    class ModelAnimation 
    {
    public:
        explicit ModelAnimation(const aiScene* scene);
        void Update(float deltaTime);
        bool SetAnimation(size_t index);
        bool SetAnimation(const std::string& name);

        const std::vector<glm::mat4>& GetBoneMatrices() const;
        inline size_t GetAnimationCount() const { return m_Animations.size(); }
        inline const std::vector<std::string>& GetAnimationNames() const { return m_AnimationNames; }
        inline uint32_t GetBoneCount() const { return static_cast<uint32_t>(m_BoneHierarchy.size()); }

    private:
        void ProcessMeshBones(const aiScene* scene);
        void ProcessAnimations(const aiScene* scene);
        void BuildBoneHierarchy(aiNode* node, Bone* parent);

        std::map<std::string, Bone> m_BoneHierarchy;
        Bone* m_RootBone = nullptr;

        // Todas las animaciones del modelo
        std::vector<Animation> m_Animations;
        std::vector<std::string> m_AnimationNames;
        std::optional<Animator> m_Animator;
        glm::mat4 m_GlobalInverseTransform;
    };
}