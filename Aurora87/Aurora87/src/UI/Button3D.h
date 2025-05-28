#pragma once

#include "Engine/Core/DebugGL.h"
#include "Engine/Core/Application.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Scene/TransformComponent.h"
#include "Engine/OpenGL/Mesh.h"
#include "Engine/OpenGL/Texture.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <memory>
#include <functional>

class Button3D
{
public:
    Button3D(std::shared_ptr<Engine::Entity> entity,
        std::shared_ptr<Engine::Texture> normalTex,
        std::shared_ptr<Engine::Texture> hoverTex);

    // Setter para callback onClick
    void SetOnClick(std::function<void()> callback) { m_OnClick = callback; }

    void Update(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos, GLFWwindow* window);

private:
    std::shared_ptr<Engine::Entity> m_Entity;
    std::shared_ptr<Engine::Texture> m_TextureNormal;
    std::shared_ptr<Engine::Texture> m_TextureHover;

    bool m_MousePressedLastFrame = false;
    bool m_IsMouseOver = false;
    int m_HoverCounter;

    std::function<void()> m_OnClick;
};