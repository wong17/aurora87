#pragma once

#include "Engine/Core/RenderSettings.h"
#include "Engine/Core/Application.h"
#include "Engine/OpenGL/Shader.h"
#include "Engine/OpenGL/CubemapTexture.h"
#include "Engine/OpenGL/VertexArray.h"
#include "Engine/OpenGL/VertexBuffer.h"
#include "Engine/OpenGL/VertexBufferElement.h"
#include "Engine/OpenGL/VertexBufferLayout.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>
#include <memory>
#include <iostream>

namespace Engine
{
    class Skybox
    {
    public:
        Skybox(const std::vector<std::string>& faces, std::shared_ptr<Shader> shader);

        void Draw(const glm::mat4& view, const glm::mat4& projection);
        void SetProjectionMatrix(const glm::mat4& projection);

        inline std::shared_ptr<CubemapTexture> GetTexture() const { return m_Texture; }

    private:
        void InitVertexData();

        std::shared_ptr<VertexArray> m_VertexArray = nullptr;
        std::shared_ptr<VertexBuffer> m_VertexBuffer = nullptr;
        std::shared_ptr<CubemapTexture> m_Texture = nullptr;
        std::shared_ptr<Shader> m_Shader = nullptr;
    };
}