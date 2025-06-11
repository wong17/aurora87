#pragma once

#include "Tween.h"

#include <vector>
#include <memory>

namespace Engine
{
    class LightAnimationClip 
    {
    public:
        std::vector<std::shared_ptr<ILightTween>> Tweens;

        LightAnimationClip() = default;

        void Update(float dt) 
        {
            for (auto& tween : Tweens) 
            {
                if (tween->IsActive())
                    tween->Update(dt);
            }
        }
    };
}