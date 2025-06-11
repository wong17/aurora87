#pragma once

#include "ILightTween.h"
#include "LightManager.h"

#include <glm/glm.hpp>

#include <cmath>

namespace Engine 
{
    class PulseTween : public ILightTween 
    {
    public:
        PulseTween(int lightIndex, LightManager* lightManager,
            float minIntensity, float maxIntensity, float speed)
            : m_Index(lightIndex), m_LightManager(lightManager),
            m_MinI(minIntensity), m_MaxI(maxIntensity), m_Speed(speed),
            m_Elapsed(0.0f)
        {
            auto pl = m_LightManager->GetPointLight(m_Index);
            m_BaseColor = glm::vec3(pl.Diffuse);
        }

        void Update(float dt) override 
        {
            m_Elapsed += dt;

            const float TWO_PI = 6.28318530718f;
            float phase = std::fmod(m_Elapsed * m_Speed, 1.0f);

            float angle = m_Elapsed * m_Speed * TWO_PI;
            float s = std::sin(angle);
            float t = 0.5f * (1.0f + s);

            float intensity = m_MinI + (m_MaxI - m_MinI) * t;

            auto pl = m_LightManager->GetPointLight(m_Index);
            glm::vec3 newColor = m_BaseColor * intensity;
            pl.Diffuse = glm::vec4(newColor, 0.0f);
            m_LightManager->SetPointLight(m_Index, pl);
        }

        bool IsActive() const override { return true; }

    private:
        int m_Index;
        LightManager* m_LightManager;
        glm::vec3 m_BaseColor;
        float m_MinI, m_MaxI;
        float m_Speed;
        float m_Elapsed;
    };

}