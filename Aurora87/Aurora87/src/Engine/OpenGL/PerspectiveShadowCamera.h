#pragma once

#include "Engine/Core/DebugGL.h"
#include "Engine/OpenGL/ShadowCamera.h"
#include "Engine/OpenGL/PerspectiveCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Engine
{
	class PerspectiveShadowCamera : public PerspectiveCamera, public ShadowCamera
	{
	public:
        PerspectiveShadowCamera(float fov, float aspectRatio, float nearClip, float farClip)
            : PerspectiveCamera(fov, aspectRatio, nearClip, farClip)
        {
            if (nearClip >= farClip || fov <= 0.0f) 
            {
                throw std::invalid_argument("PerspectiveShadowCamera: near, far y fov parametros incorrectos");
            }
            m_ViewMatrix = glm::mat4(1.0f);
        }

        inline void SetLightParams(const glm::vec3& position, const glm::vec3& direction) override
        {
            SetPosition(position);
            
            // calculamos el vector up
            glm::vec3 up;
            const glm::vec3 defaultUp = glm::vec3(0.0f, 1.0f, 0.0f);
            if (glm::length(glm::cross(direction, defaultUp)) < 0.001f) 
            {
                up = glm::vec3(0.0f, 0.0f, 1.0f);
            }
            else 
            {
                up = defaultUp;
            }

            // Normalizamos dirección
            glm::vec3 dirNormalized = direction;
            if (glm::length(dirNormalized) > 0.0f) 
            {
                dirNormalized = glm::normalize(dirNormalized);
            }
            else 
            {
                dirNormalized = glm::vec3(0.0f, 0.0f, -1.0f); // Dirección por defecto (hacia adelante)
            }

            glm::quat rotation = glm::quatLookAt(dirNormalized, up);
            m_ViewMatrix = glm::mat4_cast(rotation);
            m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
        }

        inline void UpdateMatrices() override
        {
            m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
        }

        inline const glm::mat4& GetLightSpaceMatrix() const override
        {
            return m_ViewProjectionMatrix;
        }

        inline void Resize(int width, int height) override
        {
            m_AspectRatio = float(width) / float(height);
            SetProjection(m_FOV, m_AspectRatio, m_NearClip, m_FarClip);
        }
	};
}