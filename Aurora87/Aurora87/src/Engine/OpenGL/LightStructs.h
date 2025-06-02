#pragma once

#include <glm/glm.hpp>

namespace Engine
{
    struct DirectionalLight
    {
        DirectionalLight() = default;

        DirectionalLight(
            glm::vec3 direction, 
            glm::vec3 ambient, 
            glm::vec3 diffuse, 
            glm::vec3 specular)
            : Direction(glm::normalize(direction)), 
            Ambient(ambient), 
            Diffuse(diffuse), 
            Specular(specular)
        { }

        glm::vec3 Direction = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));

        glm::vec3 Ambient = glm::vec3(0.05f);
        glm::vec3 Diffuse = glm::vec3(0.8f);
        glm::vec3 Specular = glm::vec3(1.0f);
    };

    struct PointLight
    {
        PointLight() = default;

        PointLight(
            glm::vec3 position, 
            float constant = 1.0f, 
            float linear = 0.09f, 
            float quadractic = 0.032f, 
            glm::vec3 ambient = glm::vec3(0.05f), 
            glm::vec3 diffuse = glm::vec3(0.8f), 
            glm::vec3 specular = glm::vec3(1.0f))
            : Position(position), 
            Constant(constant), 
            Linear(linear), 
            Quadratic(quadractic), 
            Ambient(ambient), 
            Diffuse(diffuse), 
            Specular(specular)
        { }

        glm::vec3 Position = glm::vec3(0.0f);
        
        float Constant = 1.0f;
        float Linear = 0.09f;
        float Quadratic = 0.032f;
        
        glm::vec3 Ambient = glm::vec3(0.05f);
        glm::vec3 Diffuse = glm::vec3(0.8f);
        glm::vec3 Specular = glm::vec3(1.0f);
    };

    struct SpotLight
    {
        SpotLight() = default;

        SpotLight(
            glm::vec3 position,
            glm::vec3 direction,
            float innerCutDeg = 12.5f,
            float outerCutDeg = 15.0f,
            float constant = 1.0f,
            float linear = 0.09f,
            float quadratic = 0.032f,
            glm::vec3 ambient = glm::vec3(0.0f),
            glm::vec3 diffuse = glm::vec3(1.0f),
            glm::vec3 specular = glm::vec3(1.0f)
        )
            : Position(position)
            , Direction(glm::normalize(direction))
            , CutOff(glm::cos(glm::radians(innerCutDeg)))
            , OuterCutOff(glm::cos(glm::radians(outerCutDeg)))
            , Constant(constant)
            , Linear(linear)
            , Quadratic(quadratic)
            , Ambient(ambient)
            , Diffuse(diffuse)
            , Specular(specular)
        { }

        glm::vec3 Position = glm::vec3(0.0f);
        glm::vec3 Direction = glm::vec3(0.0f, -1.0f, 0.0f);
        float CutOff = glm::cos(glm::radians(12.5f));
        float OuterCutOff = glm::cos(glm::radians(15.0f));
        
        float Constant = 1.0f;
        float Linear = 0.09f;
        float Quadratic = 0.032f;

        glm::vec3 Ambient = glm::vec3(0.0f);
        glm::vec3 Diffuse = glm::vec3(1.0f);
        glm::vec3 Specular = glm::vec3(1.0f);
    };
}