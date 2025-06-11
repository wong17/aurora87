#pragma once

#include "ILightTween.h"

#include <glm/glm.hpp>

#include <algorithm>
#include <type_traits>
#include <iostream>
#include <functional>

namespace Engine
{
    template<typename T>
    class Tween : public ILightTween 
    {
    public:
        T StartValue;
        T EndValue;
        float Duration;
        float Elapsed = 0.0f;
        bool Active = true;
        std::function<void(const T&)> Setter;
        std::function<T(const T&, const T&, float)> LerpFunc;
        std::function<void()> OnComplete = nullptr;

        Tween(const T& start, const T& end, float duration,
            std::function<void(const T&)> setter,
            std::function<T(const T&, const T&, float)> lerp = nullptr,
            std::function<void()> onComplete = nullptr)
            : StartValue(start), EndValue(end), Duration(duration),
            Setter(setter), OnComplete(onComplete)
        {
            if (lerp) 
            {
                LerpFunc = lerp;
            }
            else 
            {
                LerpFunc = [](auto const& a, auto const& b, float t) {
                    return a + (b - a) * t;
                    };
            }
        }

        void Update(float dt) override 
        {
            if (!Active) return;

            Elapsed += dt;
            float t = Duration > 0.0f ? glm::clamp(Elapsed / Duration, 0.0f, 1.0f) : 1.0f;
            T current = LerpFunc(StartValue, EndValue, t);

            if constexpr (std::is_same_v<T, float>) {
                // std::cout << "[Tween] t="<<t<<" cur="<<current<<"\n";
            }
            else if constexpr (std::is_same_v<T, glm::vec3>) {
                // std::cout << "[Tween] t="<<t<<" cur="<<current.x<<","<<current.y<<","<<current.z<<"\n";
            }

            Setter(current);
            if (t >= 1.0f) 
            {
                Active = false;
                if (OnComplete) OnComplete();
            }
        }

        bool IsActive() const override { return Active; }
    };
}