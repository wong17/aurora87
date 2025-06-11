#pragma once

#include "ILightTween.h"
#include "LightManager.h"

#include <glm/glm.hpp>

#include <random>
#include <functional>

namespace Engine
{
    template<typename LightType>
    class FlickerTween : public ILightTween 
    {
    public:
        FlickerTween(std::function<LightType()> getter, std::function<void(const LightType&)> setter, float variance = 0.2f)
            : m_GetLight(std::move(getter)), m_SetLight(std::move(setter)), m_Variance(variance),
            m_Eng(std::random_device{}()), m_Dist(-variance, variance)
        {
            try 
            {
                LightType lt = m_GetLight();
                m_BaseColor = glm::vec3(lt.Diffuse);
            }
            catch (const std::exception& e) 
            {
                std::cerr << "FlickerTween::FlickerTween: Error al capturar color base: " << e.what() << "\n";
                m_BaseColor = glm::vec3(1.0f);
            }
        }

        void Update(float dt) override 
        {
            float noise = m_Dist(m_Eng);
            float intensity = 1.0f + noise;

            float maxClamp = 1.0f + m_Variance;
            if (intensity < 0.0f) intensity = 0.0f;
            else if (intensity > maxClamp) intensity = maxClamp;

            try 
            {
                LightType lt = m_GetLight();
                glm::vec3 newColor = m_BaseColor * intensity;
                lt.Diffuse = glm::vec4(newColor, lt.Diffuse.a);
                m_SetLight(lt);
            }
            catch (const std::exception& e) 
            {
                std::cerr << "FlickerTween::Update: Error al obtener/establecer luz: " << e.what() << "\n";
            }
        }

        bool IsActive() const override { return true; }

    private:
        std::function<LightType()> m_GetLight;
        std::function<void(const LightType&)> m_SetLight;
        float m_Variance;
        glm::vec3 m_BaseColor;

        std::mt19937 m_Eng;
        std::uniform_real_distribution<float> m_Dist;
    };
}