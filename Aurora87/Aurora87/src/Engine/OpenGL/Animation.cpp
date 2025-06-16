#include "Animation.h"

namespace Engine
{
	Animation::Animation(const aiAnimation* animation)
	{
        if (!animation) 
        {
            throw std::invalid_argument("Animation::Constructor: aiAnimation* is null.");
        }

        m_Duration = static_cast<float>(animation->mDuration);
        m_TicksPerSecond = static_cast<float>(animation->mTicksPerSecond != 0 ? animation->mTicksPerSecond : 25.0f);
        
        if (animation->mNumChannels == 0) 
        {
            std::cerr << "Animation::Constructor: aiAnimation has no animation channels.\n";
        }

        m_Channels.reserve(animation->mNumChannels);
        for (uint32_t i = 0; i < animation->mNumChannels; ++i) 
        {
            const aiNodeAnim* channel = animation->mChannels[i];
            if (!channel) 
            {
                std::cerr << "Animation::Constructor: channel " << i << " is null, is omitted.\n";
                continue;
            }
            ReadNodeAnimation(channel);
        }

        if (m_Channels.empty()) 
        {
            std::cerr << "Animation::Constructor: No valid node animation was loaded..\n";
        }
	}

	void Animation::ReadNodeAnimation(const aiNodeAnim* channel)
	{
        if (!channel) 
        {
            std::cerr << "Animation::ReadNodeAnimation: aiNodeAnim* is null.\n";
            return;
        }

        NodeAnimation nodeAnim;
        nodeAnim.NodeName = channel->mNodeName.C_Str();
        if (nodeAnim.NodeName.empty()) 
        {
            nodeAnim.NodeName = "<UnnamedNode>";
            std::cerr << "Animation::ReadNodeAnimation: unnamed channel, ‘<UnnamedNode>’ is assigned.\n";
        }

        nodeAnim.Positions.reserve(channel->mNumPositionKeys);
        nodeAnim.Rotations.reserve(channel->mNumRotationKeys);
        nodeAnim.Scales.reserve(channel->mNumScalingKeys);

        // Posiciones
        if (channel->mNumPositionKeys == 0) 
        {
            std::cerr << "Animation::ReadNodeAnimation (" << nodeAnim.NodeName << ") without PositionKeys.\n";
        }

        for (uint32_t i = 0; i < channel->mNumPositionKeys; ++i) 
        {
            const auto& pk = channel->mPositionKeys[i];
            PositionKey k;
            k.Time = static_cast<float>(pk.mTime);
            k.Value = glm::make_vec3(&pk.mValue.x);
            nodeAnim.Positions.push_back(k);
        }

        // Rotaciones
        if (channel->mNumRotationKeys == 0) 
        {
            std::cerr << "Animation::ReadNodeAnimation (" << nodeAnim.NodeName << ") without RotationKeys.\n";
        }

        for (uint32_t i = 0; i < channel->mNumRotationKeys; ++i) 
        {
            const auto& rk = channel->mRotationKeys[i];
            RotationKey k;
            k.Time = static_cast<float>(rk.mTime);
            auto& v = rk.mValue;
            k.Value = glm::normalize(glm::quat(v.w, v.x, v.y, v.z));
            nodeAnim.Rotations.push_back(k);
        }

        // Escalas
        if (channel->mNumScalingKeys == 0) 
        {
            std::cerr << "Animation::ReadNodeAnimation (" << nodeAnim.NodeName << ") without ScalingKeys.\n";
        }
        for (uint32_t i = 0; i < channel->mNumScalingKeys; ++i) 
        {
            const auto& sk = channel->mScalingKeys[i];
            ScaleKey k;
            k.Time = static_cast<float>(sk.mTime);
            k.Value = glm::make_vec3(&sk.mValue.x);
            nodeAnim.Scales.push_back(k);
        }

        // Solo agregamos si hay al menos un key en alguna categoría
        if (nodeAnim.Positions.empty() && nodeAnim.Rotations.empty() && nodeAnim.Scales.empty()) 
        {
            std::cerr << "Animation::ReadNodeAnimation: channel '" << nodeAnim.NodeName << "' without keyframes, is omitted.\n";
            return;
        }

        m_Channels.push_back(std::move(nodeAnim));
	}
}