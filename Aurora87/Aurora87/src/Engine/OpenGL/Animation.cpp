#include "Animation.h"

namespace Engine
{
	Animation::Animation(const aiAnimation* animation)
	{
        if (!animation) 
        {
            throw std::invalid_argument("Animation::Constructor: aiAnimation* es nulo.");
        }

        m_Duration = static_cast<float>(animation->mDuration);
        m_TicksPerSecond = static_cast<float>(animation->mTicksPerSecond != 0 ? animation->mTicksPerSecond : 25.0f);
        
        if (animation->mNumChannels == 0) 
        {
            std::cerr << "Animation::Constructor: aiAnimation no tiene canales de animacion.\n";
        }

        m_Channels.reserve(animation->mNumChannels);
        for (uint32_t i = 0; i < animation->mNumChannels; ++i) 
        {
            const aiNodeAnim* channel = animation->mChannels[i];
            if (!channel) 
            {
                std::cerr << "Animation::Constructor: canal " << i << " es nulo, se omite.\n";
                continue;
            }
            ReadNodeAnimation(channel);
        }

        if (m_Channels.empty()) 
        {
            std::cerr << "Animation::Constructor: No se cargo ninguna animacion de nodo valida.\n";
        }
	}

	void Animation::ReadNodeAnimation(const aiNodeAnim* channel)
	{
        if (!channel) 
        {
            std::cerr << "Animation::ReadNodeAnimation: aiNodeAnim* es nulo.\n";
            return;
        }

        NodeAnimation nodeAnim;
        nodeAnim.NodeName = channel->mNodeName.C_Str();
        if (nodeAnim.NodeName.empty()) 
        {
            nodeAnim.NodeName = "<UnnamedNode>";
            std::cerr << "Animation::ReadNodeAnimation: canal sin nombre, se asigna '<UnnamedNode>'.\n";
        }

        nodeAnim.Positions.reserve(channel->mNumPositionKeys);
        nodeAnim.Rotations.reserve(channel->mNumRotationKeys);
        nodeAnim.Scales.reserve(channel->mNumScalingKeys);

        // Posiciones
        if (channel->mNumPositionKeys == 0) 
        {
            std::cerr << "Animation::ReadNodeAnimation (" << nodeAnim.NodeName << ") sin PositionKeys.\n";
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
            std::cerr << "Animation::ReadNodeAnimation (" << nodeAnim.NodeName << ") sin RotationKeys.\n";
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
            std::cerr << "Animation::ReadNodeAnimation (" << nodeAnim.NodeName << ")sin ScalingKeys.\n";
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
            std::cerr << "Animation::ReadNodeAnimation: canal '" << nodeAnim.NodeName << "' sin keyframes, se omite.\n";
            return;
        }

        m_Channels.push_back(std::move(nodeAnim));
	}
}