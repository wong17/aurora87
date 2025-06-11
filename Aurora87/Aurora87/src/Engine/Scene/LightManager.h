#pragma once

#include "Engine/OpenGL/UniformBuffer.h"
#include "Engine/OpenGL/Shader.h"
#include "Engine/OpenGL/LightStructs.h"

#include "GlobalSceneSettings.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <memory>

namespace Engine
{
	class LightManager
	{
    public:
        LightManager(std::shared_ptr<UniformBuffer> lightUBO, int maxDir, int maxPoint, int maxSpot);

        int AddDirectionalLight(const DirectionalLight& dl);
        int AddPointLight(const PointLight& pl);
        int AddSpotLight(const SpotLight& sl);

        void SetDirectionalLight(int index, const DirectionalLight& dl);
        void SetPointLight(int index, const PointLight& pl);
        void SetSpotLight(int index, const SpotLight& sl);
        void SetGlobalAmbient(const glm::vec3& ambientColor);

        void PopLastDirectionalLight();
        void PopLastPointLight();
        void PopLastSpotLight();

        void UpdateSceneLights();

        inline glm::vec3 GetGlobalAmbientColor() const 
        { 
            return glm::vec3(m_GlobalAmbientColor.r, m_GlobalAmbientColor.g, m_GlobalAmbientColor.b);
        }
        inline int GetMaxDirLights() const { return m_MaxDirLights; }
        inline int GetMaxPointLights() const { return m_MaxPointLights; }
        inline int GetMaxSpotLights() const { return m_MaxSpotLights; }

        PointLight GetPointLight(int index) const;
        DirectionalLight GetDirectionalLight(int index) const;
        SpotLight GetSpotLight(int index) const;

    private:
        std::shared_ptr<UniformBuffer> m_LightUBO = nullptr;

        std::vector<DirectionalLight> m_DirectionalLights;
        std::vector<PointLight> m_PointLights;
        std::vector<SpotLight> m_SpotLights;

		glm::vec4 m_GlobalAmbientColor = glm::vec4(0.005f, 0.005f, 0.005f, 0.0f);

        int m_MaxDirLights = 0;
        int m_MaxPointLights = 0;
        int m_MaxSpotLights = 0;
	};
}