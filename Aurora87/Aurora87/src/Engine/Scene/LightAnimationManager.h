#pragma once

#include "LightAnimationClip.h"

#include <vector>
#include <memory>

namespace Engine
{
    class LightAnimationManager 
    {
    public:
        std::vector<std::shared_ptr<LightAnimationClip>> Clips;

        void AddClip(const std::shared_ptr<LightAnimationClip>& clip) 
        {
            Clips.push_back(clip);
        }

        void Update(float dt) 
        {
            for (auto& clip : Clips) 
            {
                clip->Update(dt);
            }
            
            Clips.erase(
                std::remove_if(Clips.begin(), Clips.end(),
                    [](auto& clip) 
                    {
                        bool anyActive = false;
                        for (auto& t : clip->Tweens) 
                        {
                            if (t->IsActive()) { anyActive = true; break; }
                        }
                        return !anyActive;
                    }),
                Clips.end());
        }
    };
}