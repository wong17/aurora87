#pragma once

#include "Animation.h"
#include "Bone.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <assimp/anim.h>
#include <assimp/scene.h>

#include <map>
#include <string>
#include <vector>
#include <cstdint>
#include <cmath>
#include <iostream>
#include <utility>

namespace Engine
{
    class Animator 
    {
    public:
        Animator(const Animation& animation,
            std::map<std::string, Bone>& bones,
            Bone* rootBone,
            const glm::mat4& globalInverse);

        void Update(float deltaTime);

        // Para enviar al shader
        const std::vector<glm::mat4>& GetFinalBoneMatrices() const { return m_FinalBoneMatrices; }

    private:
        // Para cada canal, interpola y escribe en la estructura Bone
        template<typename Key>
        static void Interpolate(const std::vector<Key>& keys, float time, typename Key::ValueType& outValue);

        const Animation& m_Animation;
        std::map<std::string, Bone>& m_Bones;
        Bone* m_RootBone;
        glm::mat4 m_GlobalInverseTransform;

        float m_CurrentTime = 0.0f;
        std::vector<std::pair<Bone*, const NodeAnimation*>> m_BoneChannels;
        std::vector<glm::mat4> m_FinalBoneMatrices;
    };
}