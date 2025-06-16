#include "Animator.h"

namespace Engine
{
    Animator::Animator(const Animation& animation,
        std::map<std::string, Bone>& bones,
        Bone* rootBone,
        const glm::mat4& globalInverse)
        : m_Animation(animation)
        , m_Bones(bones)
        , m_RootBone(rootBone)
        , m_GlobalInverseTransform(globalInverse)
    {
        if (!m_RootBone)
            throw std::invalid_argument("Animator::Animator: rootBone is null");

        // Cache only channels that have associated bone
        for (const auto& ch : m_Animation.Channels()) 
        {
            auto it = m_Bones.find(ch.NodeName);
            if (it != m_Bones.end()) 
            {
                m_BoneChannels.emplace_back(&it->second, &ch);
            }
        }
        m_FinalBoneMatrices.resize(m_Bones.size(), glm::mat4(1.0f));
    }

    void Animator::Update(float deltaTime) 
    {
        if (deltaTime <= 0.0f) 
        {
            std::cerr << "Animator::Update: deltaTime is not positive\n";
            return;
        }

        constexpr float MAX_DELTA = 0.1f;
        deltaTime = std::min(deltaTime, MAX_DELTA);

        float ticks = deltaTime * m_Animation.TicksPerSecond();
        m_CurrentTime = std::fmod(m_CurrentTime + ticks, m_Animation.Duration());

        for (auto& [bone, channel] : m_BoneChannels) 
        {
            // Position
            glm::vec3 T(0.0f);
            Interpolate(channel->Positions, m_CurrentTime, T);
            // Rotation
            glm::quat R(1.0f, 0.0f, 0.0f, 0.0f);
            Interpolate(channel->Rotations, m_CurrentTime, R);
            // Scale
            glm::vec3 S(1.0f);
            Interpolate(channel->Scales, m_CurrentTime, S);

            bone->animatedTransform = 
                glm::translate(glm::mat4(1.0f), T) * glm::mat4_cast(R) * glm::scale(glm::mat4(1.0f), S);
        }

        // Filling the array of final arrays
        m_FinalBoneMatrices.assign(m_Bones.size(), glm::mat4(1.0f));
        m_RootBone->Update(
            glm::mat4(1.0f),
            m_GlobalInverseTransform,
            m_FinalBoneMatrices
        );
    }

    template<typename Key>
    static void Animator::Interpolate(const std::vector<Key>& keys, float time, typename Key::ValueType& out) 
    {
        if (keys.empty()) return;
        
        if (time <= keys.front().Time) 
        {
            out = keys.front().Value;
            return;
        }

        if (time >= keys.back().Time) 
        {
            out = keys.back().Value;
            return;
        }
        
        for (size_t i = 0; i + 1 < keys.size(); ++i) 
        {
            const auto& A = keys[i];
            const auto& B = keys[i + 1];
            if (time < B.Time) 
            {
                float factor = (time - A.Time) / (B.Time - A.Time);
                if constexpr (std::is_same_v<typename Key::ValueType, glm::quat>) 
                {
                    out = glm::slerp(A.Value, B.Value, factor);
                }
                else 
                {
                    out = glm::mix(A.Value, B.Value, factor);
                }
                return;
            }
        }

        out = keys.back().Value;
    }
}