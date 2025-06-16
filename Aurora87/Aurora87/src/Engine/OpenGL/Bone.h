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
        glm::mat4 localBindPose = glm::mat4(1.0f);       // Local transformation in bind pose
        glm::mat4 inverseBindMatrix = glm::mat4(1.0f);   // Offset matrix
        glm::mat4 animatedTransform = glm::mat4(1.0f);   // Current transformation during animation
        uint32_t index = 0;                              // Index in the array of arrays

        void Update(const glm::mat4& parentTransform,
            const glm::mat4& globalInverseTransform,
            std::vector<glm::mat4>& outFinalMatrices)
        {
            // Composing the global transformation of this bone
            glm::mat4 globalTransform = parentTransform * localBindPose * animatedTransform;

            // Write the final matrix (offset included) in the vector
            outFinalMatrices[index] = globalInverseTransform * globalTransform * inverseBindMatrix;

            for (Bone* child : children)
            {
                child->Update(globalTransform, globalInverseTransform, outFinalMatrices);
            }
        }
    };
}