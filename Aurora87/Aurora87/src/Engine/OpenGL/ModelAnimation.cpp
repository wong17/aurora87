#include "ModelAnimation.h"

namespace Engine
{
    ModelAnimation::ModelAnimation(const aiScene* scene)
    {
        if (!scene || !scene->mRootNode)
        {
            throw std::invalid_argument("ModelAnimation::ModelAnimation: escena o nodo raíz nulo.");
        }

        m_GlobalInverseTransform = glm::inverse(glm::make_mat4(&scene->mRootNode->mTransformation.a1));

        ProcessMeshBones(scene);
        ProcessAnimations(scene);

        if (!m_RootBone)
            throw std::runtime_error("ModelAnimation::ModelAnimation: no se encontro rootBone en la jerarquia de nodos.");

		// Si hay animaciones, inicializa el Animator con la primera animación por defecto
        if (!m_Animations.empty())
            SetAnimation(0);
    }

    void ModelAnimation::ProcessMeshBones(const aiScene* scene)
    {
        if (!scene) return;

        for (uint32_t m = 0; m < scene->mNumMeshes; ++m) 
        {
            const aiMesh* mesh = scene->mMeshes[m];
            if (!mesh) continue;

            for (uint32_t b = 0; b < mesh->mNumBones; ++b) 
            {
                const aiBone* aiBone = mesh->mBones[b];
                if (!aiBone) continue;

                std::string name = aiBone->mName.C_Str();
                if (name.empty()) continue;

                // Crear o actualizar hueso
                auto it = m_BoneHierarchy.find(name);
                if (it == m_BoneHierarchy.end())
                {
                    Bone bone;
                    bone.name = name;
                    bone.inverseBindMatrix = glm::make_mat4(&aiBone->mOffsetMatrix.a1);
                    bone.index = static_cast<uint32_t>(m_BoneHierarchy.size());
                    m_BoneHierarchy[name] = bone;
                }
            }
        }
        BuildBoneHierarchy(scene->mRootNode, nullptr);
    }

    void ModelAnimation::ProcessAnimations(const aiScene* scene)
    {
        if (!scene || scene->mNumAnimations == 0)
            return;

        m_Animations.reserve(scene->mNumAnimations);
        m_AnimationNames.reserve(scene->mNumAnimations);

        for (uint32_t i = 0; i < scene->mNumAnimations; ++i) 
        {
            const aiAnimation* anim = scene->mAnimations[i];
            if (!anim) continue;

            m_Animations.emplace_back(anim);
            std::string name = anim->mName.C_Str();
            if (name.empty()) name = "Anim_" + std::to_string(i);
            m_AnimationNames.push_back(name);
        }
    }

    void ModelAnimation::BuildBoneHierarchy(aiNode* node, Bone* parent)
    {
        if (!node) return;

        std::string nodeName = node->mName.C_Str();
        Bone* currentBone = nullptr;

        // Buscar si ya existe en el mapa
        auto it = m_BoneHierarchy.find(nodeName);
        if (it != m_BoneHierarchy.end()) 
        {
            currentBone = &it->second;
            currentBone->parent = parent;
            currentBone->localBindPose = glm::make_mat4(&node->mTransformation.a1);

            if (parent) 
            {
                parent->children.push_back(currentBone);
            }
            else 
            {
                m_RootBone = currentBone;
            }

			parent = currentBone; // Actualizar padre para hijos
        }
        else
        {
            std::cerr << "ModelAnimation::BuildBoneHierarchy: nodo '"
                << nodeName << "' no corresponde a ningun hueso de mesh.\n";
        }

        // Procesar hijos recursivamente
        for (uint32_t i = 0; i < node->mNumChildren; ++i) 
        {
            BuildBoneHierarchy(node->mChildren[i], parent);
        }
    }

    bool ModelAnimation::SetAnimation(size_t index)
    {
        if (index >= m_Animations.size()) 
            return false;

        // Recrea el Animator con la animación seleccionada
        m_Animator.emplace(
            m_Animations[index],
            m_BoneHierarchy,  // Pasar mapa completo
            m_RootBone,       // Pasar raíz jerárquica
            m_GlobalInverseTransform
        );

        return true;
    }

    bool ModelAnimation::SetAnimation(const std::string& name)
    {
        if (name.empty())
            return false;

        auto it = std::find(m_AnimationNames.begin(), m_AnimationNames.end(), name);
        if (it == m_AnimationNames.end()) 
            return false;
        
        return SetAnimation(std::distance(m_AnimationNames.begin(), it));
    }

    void ModelAnimation::Update(float deltaTime)
    {
        if (m_Animator && deltaTime > 0.0f)
            m_Animator->Update(deltaTime);
    }

    const std::vector<glm::mat4>& ModelAnimation::GetBoneMatrices() const
    {
        static const std::vector<glm::mat4> empty;
        return m_Animator ? m_Animator->GetFinalBoneMatrices() : empty;
    }
}