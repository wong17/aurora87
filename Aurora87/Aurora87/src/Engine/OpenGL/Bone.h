#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <cstdint>

namespace Engine
{
    struct Bone 
    {
        std::string name;
        Bone* parent = nullptr;
        std::vector<Bone*> children;
        glm::mat4 localBindPose = glm::mat4(1.0f);       // Transformación local en pose bind
        glm::mat4 inverseBindMatrix = glm::mat4(1.0f);   // Matriz offset
        glm::mat4 animatedTransform = glm::mat4(1.0f);   // Transformación actual durante animación
        uint32_t index = 0;                              // Índice en el array de matrices

        void Update(const glm::mat4& parentTransform,
            const glm::mat4& globalInverseTransform,
            std::vector<glm::mat4>& outFinalMatrices)
        {
            // Componer la transformación global de este hueso
            glm::mat4 globalTransform = parentTransform * localBindPose * animatedTransform;

            // Escribir la matriz final (offset incluida) en el vector
            outFinalMatrices[index] = globalInverseTransform * globalTransform * inverseBindMatrix;

            for (Bone* child : children)
            {
                child->Update(globalTransform, globalInverseTransform, outFinalMatrices);
            }
        }
    };
}