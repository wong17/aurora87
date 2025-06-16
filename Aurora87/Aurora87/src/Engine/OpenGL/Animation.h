#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/scene.h>

#include <cstdint>
#include <vector>
#include <stdexcept>
#include <iostream>

namespace Engine
{
    // Position keyframes
    struct PositionKey 
    {
        using ValueType = glm::vec3;
        ValueType Value;
        float Time;
    };

    // Rotation keyframes
    struct RotationKey 
    {
        using ValueType = glm::quat;
        ValueType Value;
        float Time;
    };

    // Scale keyframes
    struct ScaleKey 
    {
        using ValueType = glm::vec3;
        ValueType Value;
        float Time;
    };

    // Animation channel per node
    struct NodeAnimation 
    {
        std::string NodeName;
        std::vector<PositionKey> Positions;
        std::vector<RotationKey> Rotations;
        std::vector<ScaleKey> Scales;
    };

    class Animation 
    {
    public:
        explicit Animation(const aiAnimation* animation);

        inline float Duration() const { return m_Duration; }
        inline float TicksPerSecond() const { return m_TicksPerSecond; }
        inline const std::vector<NodeAnimation>& Channels() const { return m_Channels; }

    private:
        void ReadNodeAnimation(const aiNodeAnim* channel);

        float m_Duration;
        float m_TicksPerSecond;
        std::vector<NodeAnimation> m_Channels;
    };
}