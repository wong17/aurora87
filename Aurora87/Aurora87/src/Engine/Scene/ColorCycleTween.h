#pragma once

#include "ILightTween.h"

#include <glm/glm.hpp>

#include <functional>
#include <cmath>

namespace Engine 
{

    class ColorCycleTween : public ILightTween 
    {
    public:
        ColorCycleTween(float speed, std::function<void(const glm::vec3&)> setter)
            : m_Speed(speed), Setter(setter), m_Elapsed(0.0f) { }

        void Update(float dt) override 
        {
            m_Elapsed += dt;
            float hue = std::fmod(m_Elapsed * m_Speed, 1.0f);
            glm::vec3 rgb = HSVtoRGB(hue, 1.0f, 1.0f);
            Setter(rgb);
        }

        bool IsActive() const override { return true; }

    private:
        float m_Speed;
        float m_Elapsed;
        std::function<void(const glm::vec3&)> Setter;

        glm::vec3 HSVtoRGB(float h, float s, float v) 
        {
            float h6 = h * 6.0f;
            int i = static_cast<int>(std::floor(h6)) % 6;
            float f = h6 - std::floor(h6);
            float p = v * (1.0f - s);
            float q = v * (1.0f - f * s);
            float t = v * (1.0f - (1.0f - f) * s);
            
            switch (i) 
            {
                case 0: return glm::vec3(v, t, p);
                case 1: return glm::vec3(q, v, p);
                case 2: return glm::vec3(p, v, t);
                case 3: return glm::vec3(p, q, v);
                case 4: return glm::vec3(t, p, v);
                case 5: default: return glm::vec3(v, p, q);
            }
        }
    };

}