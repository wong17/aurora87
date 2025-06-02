#pragma once

#include <atomic>

namespace Engine
{
	extern std::atomic<bool> g_EnableShadows;

    struct LightCounts
    {
        int MaxDirectionalLights = 0;
        int MaxPointLights = 0;
        int MaxSpotLights = 0;
    };
    extern LightCounts g_LightCounts;

    static constexpr uint32_t BINDING_CAMERA = 0;
    static constexpr uint32_t BINDING_MODEL = 1;
    static constexpr uint32_t BINDING_BONES = 2;
    static constexpr uint32_t BINDING_TEXTUREBLOCK = 3;
    static constexpr uint32_t BINDING_LIGHTBLOCK = 4;
}