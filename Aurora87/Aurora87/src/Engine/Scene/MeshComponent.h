#pragma once

#include "Engine/OpenGL/Mesh.h"

#include <memory>
#include <cstdint>

namespace Engine
{
    struct MeshComponent 
    {
        std::shared_ptr<Engine::Mesh> MeshPtr;
        uint32_t InstanceCount = 0;
    };
}