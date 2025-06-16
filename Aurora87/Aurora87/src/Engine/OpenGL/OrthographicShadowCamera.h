#pragma once

#include "Engine/Core/DebugGL.h"
#include "Engine/OpenGL/ShadowCamera.h"
#include "Engine/OpenGL/OrthographicCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdexcept>

namespace Engine
{
	class OrthographicShadowCamera : public OrthographicCamera, public ShadowCamera
	{
	public:
        OrthographicShadowCamera(float left, float right, float bottom, float top,
            float nearPlane = 0.1f, float farPlane = 10.0f)
            : OrthographicCamera(left, right, bottom, top), m_Near(nearPlane), m_Far(farPlane)
        {
            // when instantiating, we overwrite the projection to use the projection of this class that includes near/far
            SetProjection(left, right, bottom, top);
            m_ViewMatrix = glm::mat4(1.0f);
        }

        inline void SetLightParams(const glm::vec3& position, const glm::vec3& direction) override
        {
            // position
            SetPosition(position);
            
            // calculate the up vector
            glm::vec3 up;
            const glm::vec3 defaultUp = glm::vec3(0.0f, 1.0f, 0.0f);
            if (glm::length(glm::cross(direction, defaultUp)) < 0.001f)
            {
                up = glm::vec3(0.0f, 0.0f, 1.0f); // We use Z if the direction is almost vertical
            }
            else
            {
                up = defaultUp; // General case
            }

            // we normalize direction
            glm::vec3 dirNormalized = direction;
            if (glm::length(dirNormalized) > 0.0f)
            {
                dirNormalized = glm::normalize(dirNormalized);
            }
            else
            {
                dirNormalized = glm::vec3(0.0f, 0.0f, -1.0f); // Default (forward) direction
            }

            // Calculate view matrix using lookAt
            m_ViewMatrix = glm::lookAt(position, position + dirNormalized, up);
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

        inline void SetProjection(float left, float right, float bottom, float top) override
        {
            if (left >= right || bottom >= top || m_Near >= m_Far) 
            {
                throw std::invalid_argument("OrthographicShadowCamera::SetProjection: incorrect parameters");
            }

            // reconstruimos la matriz ortográfica usando near/far
            m_ProjectionMatrix = glm::ortho(left, right, bottom, top, m_Near, m_Far);
            UpdateMatrices();
        }

        inline void SetNearFar(float nearPlane, float farPlane)
        {
            m_Near = nearPlane;
            m_Far = farPlane;
            SetProjection(m_Left, m_Right, m_Bottom, m_Top);
        }

		inline float GetNear() const { return m_Near; }
        inline float GetFar() const { return m_Far; }

    private:
        float m_Near, m_Far;
	};
}