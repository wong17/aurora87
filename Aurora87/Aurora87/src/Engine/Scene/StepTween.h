#pragma once

#include "ILightTween.h"

#include <glm/glm.hpp>

#include <functional>
#include <type_traits>
#include <iostream>

namespace Engine
{
    template<typename T>
    class StepTween : public ILightTween 
    {
    public:
        T Current;
        T EndValue;
        T StepPerSecond;
        bool Active = true;
        std::function<void(const T&)> Setter;
        std::function<void()> OnComplete = nullptr;

        StepTween(const T& start, const T& end, const T& stepPerSecond,
            std::function<void(const T&)> setter,
            std::function<void()> onComplete = nullptr)
            : Current(start), EndValue(end),
            StepPerSecond(stepPerSecond),
            Setter(setter), OnComplete(onComplete)
        {
            if (HasReachedOrPassed(Current)) 
            {
                Current = EndValue;
                Setter(Current);
                Active = false;
                if (OnComplete) OnComplete();
            }
        }

        void Update(float dt) override 
        {
            if (!Active) return;

            T next;
            if constexpr (std::is_same_v<T, float>) 
            {
                next = Current + StepPerSecond * dt;
                if ((StepPerSecond > 0.0f && next >= EndValue) ||
                    (StepPerSecond < 0.0f && next <= EndValue)) 
                {
                    next = EndValue;
                    Active = false;
                }
            }
            else if constexpr (std::is_same_v<T, glm::vec3>) 
            {
                next = Current + StepPerSecond * dt;
                glm::vec3 c = Current;
                glm::vec3 e = EndValue;
                glm::vec3 s = StepPerSecond;
                glm::vec3 n = next;
                
                glm::vec3 clamped = n;
                bool done = true;
                for (int i = 0; i < 3; ++i) 
                {
                    if (s[i] > 0.0f) 
                    {
                        if (n[i] >= e[i]) clamped[i] = e[i];
                        else done = false;
                    }
                    else if (s[i] < 0.0f) 
                    {
                        if (n[i] <= e[i]) clamped[i] = e[i];
                        else done = false;
                    }
                    else 
                    {
                        if (c[i] != e[i]) done = false;
                    }
                }
                next = clamped;
                if (done) Active = false;
            }
            else 
            {
                next = Current;
                Active = false;
            }

            Current = next;
            if constexpr (std::is_same_v<T, float>) 
            {
                // std::cout<<"[StepTween] cur="<<Current<<"\n";
            }
            else if constexpr (std::is_same_v<T, glm::vec3>) 
            {
                // std::cout<<"[StepTween] cur="<<Current.x<<","<<Current.y<<","<<Current.z<<"\n";
            }

            Setter(Current);
            if (!Active && OnComplete) OnComplete();
        }

        bool IsActive() const override { return Active; }

    private:
        bool HasReachedOrPassed(const float& v) const 
        {
            if (StepPerSecond > 0.0f) return v >= EndValue;
            if (StepPerSecond < 0.0f) return v <= EndValue;

            return v == EndValue;
        }

        bool HasReachedOrPassed(const glm::vec3& v) const 
        {
            for (int i = 0; i < 3; ++i) 
            {
                if (StepPerSecond[i] > 0.0f) 
                {
                    if (v[i] < EndValue[i]) return false;
                }
                else if (StepPerSecond[i] < 0.0f) 
                {
                    if (v[i] > EndValue[i]) return false;
                }
                else 
                {
                    if (v[i] != EndValue[i]) return false;
                }
            }

            return true;
        }
    };
}