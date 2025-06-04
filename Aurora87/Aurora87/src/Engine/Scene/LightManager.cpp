#include "LightManager.h"

namespace Engine
{
    LightManager::LightManager(std::shared_ptr<UniformBuffer> lightUBO, int maxDir, int maxPoint, int maxSpot)
        : m_LightUBO(lightUBO), m_MaxDirLights(maxDir), m_MaxPointLights(maxPoint), m_MaxSpotLights(maxSpot),
        m_GlobalAmbientColor(0.05f, 0.05f, 0.05f)
    { }

    int LightManager::AddDirectionalLight(const DirectionalLight& dl)
    {
        if (m_DirectionalLights.size() >= m_MaxDirLights)
            return -1;

        m_DirectionalLights.push_back(dl);
        return static_cast<int>(m_DirectionalLights.size() - 1);
    }

    int LightManager::AddPointLight(const PointLight& pl)
    {
        if (m_PointLights.size() >= m_MaxPointLights)
            return -1;

        m_PointLights.push_back(pl);
        return static_cast<int>(m_PointLights.size() - 1);
    }

    int LightManager::AddSpotLight(const SpotLight& sl)
    {
        if (m_SpotLights.size() >= m_MaxSpotLights)
            return -1;

        m_SpotLights.push_back(sl);
        return static_cast<int>(m_SpotLights.size() - 1);
    }

    void LightManager::SetDirectionalLight(int index, const DirectionalLight& dl)
    {
        if (index >= 0 && index < static_cast<int>(m_DirectionalLights.size()))
            m_DirectionalLights[index] = dl;
    }

    void LightManager::SetPointLight(int index, const PointLight& pl)
    {
        if (index >= 0 && index < static_cast<int>(m_PointLights.size()))
            m_PointLights[index] = pl;
    }

    void LightManager::SetSpotLight(int index, const SpotLight& sl)
    {
        if (index >= 0 && index < static_cast<int>(m_SpotLights.size()))
            m_SpotLights[index] = sl;
    }

    void LightManager::SetGlobalAmbient(const glm::vec3& ambientColor)
    {
        m_GlobalAmbientColor = ambientColor;
    }

    void LightManager::PopLastDirectionalLight()
    {
        if (!m_DirectionalLights.empty())
            m_DirectionalLights.pop_back();
    }

    void LightManager::PopLastPointLight()
    {
        if (!m_PointLights.empty())
            m_PointLights.pop_back();
    }

    void LightManager::PopLastSpotLight()
    {
        if (!m_SpotLights.empty())
            m_SpotLights.pop_back();
    }

    void LightManager::UpdateSceneLights()
    {
        m_LightUBO->Bind();
        uint32_t offsetBase = 0;
		auto& layout = m_LightUBO->GetLayout();

        int numDir = static_cast<int>(m_DirectionalLights.size());
        int numPnt = static_cast<int>(m_PointLights.size());
        int numSpt = static_cast<int>(m_SpotLights.size());
        m_LightUBO->SetData(&numDir, sizeof(int), offsetBase + layout.GetElement("u_NumDirectionalLights").Offset);
        m_LightUBO->SetData(&numPnt, sizeof(int), offsetBase + layout.GetElement("u_NumPointLights").Offset);
        m_LightUBO->SetData(&numSpt, sizeof(int), offsetBase + layout.GetElement("u_NumSpotLights").Offset);

        m_LightUBO->SetData(glm::value_ptr(m_GlobalAmbientColor), sizeof(glm::vec4), offsetBase + layout.GetElement("u_GlobalAmbient").Offset);

        for (int i = 0; i < numDir; ++i)
        {
            const auto& L = m_DirectionalLights[i];
            std::string base = "directionalLights[" + std::to_string(i) + "]";
            m_LightUBO->SetData(glm::value_ptr(L.Direction), sizeof(glm::vec4), offsetBase + layout.GetElement(base + ".direction").Offset);
            m_LightUBO->SetData(glm::value_ptr(L.Ambient), sizeof(glm::vec4), offsetBase + layout.GetElement(base + ".ambient").Offset);
            m_LightUBO->SetData(glm::value_ptr(L.Diffuse), sizeof(glm::vec4), offsetBase + layout.GetElement(base + ".diffuse").Offset);
            m_LightUBO->SetData(glm::value_ptr(L.Specular), sizeof(glm::vec4), offsetBase + layout.GetElement(base + ".specular").Offset);
        }

        for (int i = 0; i < numPnt; ++i)
        {
            const auto& L = m_PointLights[i];
            std::string base = "pointLights[" + std::to_string(i) + "]";
            m_LightUBO->SetData(glm::value_ptr(L.Position), sizeof(glm::vec4), offsetBase + layout.GetElement(base + ".position").Offset);
            m_LightUBO->SetData(&L.Constant, sizeof(float), offsetBase + layout.GetElement(base + ".constant").Offset);
            m_LightUBO->SetData(&L.Linear, sizeof(float), offsetBase + layout.GetElement(base + ".linear").Offset);
            m_LightUBO->SetData(&L.Quadratic, sizeof(float), offsetBase + layout.GetElement(base + ".quadratic").Offset);
            m_LightUBO->SetData(glm::value_ptr(L.Ambient), sizeof(glm::vec4), offsetBase + layout.GetElement(base + ".ambient").Offset);
            m_LightUBO->SetData(glm::value_ptr(L.Diffuse), sizeof(glm::vec4), offsetBase + layout.GetElement(base + ".diffuse").Offset);
            m_LightUBO->SetData(glm::value_ptr(L.Specular), sizeof(glm::vec4), offsetBase + layout.GetElement(base + ".specular").Offset);
        }

        for (int i = 0; i < numSpt; ++i)
        {
            const auto& L = m_SpotLights[i];
            std::string base = "spotLights[" + std::to_string(i) + "]";
            m_LightUBO->SetData(glm::value_ptr(L.Position), sizeof(glm::vec4), offsetBase + layout.GetElement(base + ".position").Offset);
            m_LightUBO->SetData(glm::value_ptr(L.Direction), sizeof(glm::vec4), offsetBase + layout.GetElement(base + ".direction").Offset);
            m_LightUBO->SetData(&L.CutOff, sizeof(float), offsetBase + layout.GetElement(base + ".cutOff").Offset);
            m_LightUBO->SetData(&L.OuterCutOff, sizeof(float), offsetBase + layout.GetElement(base + ".outerCutOff").Offset);
            m_LightUBO->SetData(&L.Constant, sizeof(float), offsetBase + layout.GetElement(base + ".constant").Offset);
            m_LightUBO->SetData(&L.Linear, sizeof(float), offsetBase + layout.GetElement(base + ".linear").Offset);
            m_LightUBO->SetData(&L.Quadratic, sizeof(float), offsetBase + layout.GetElement(base + ".quadratic").Offset);
            m_LightUBO->SetData(glm::value_ptr(L.Ambient), sizeof(glm::vec4), offsetBase + layout.GetElement(base + ".ambient").Offset);
            m_LightUBO->SetData(glm::value_ptr(L.Diffuse), sizeof(glm::vec4), offsetBase + layout.GetElement(base + ".diffuse").Offset);
            m_LightUBO->SetData(glm::value_ptr(L.Specular), sizeof(glm::vec4), offsetBase + layout.GetElement(base + ".specular").Offset);
        }

        m_LightUBO->BindRange(0, m_LightUBO->GetAlignedStride());
    }
}