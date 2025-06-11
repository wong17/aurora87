#pragma once

#include <glm/glm.hpp>

namespace Engine
{
    struct alignas(16) DirectionalLight
    {
        DirectionalLight() = default;

        DirectionalLight(
            const glm::vec3& direction,
            const glm::vec3& ambient = glm::vec3(0.05f),
            const glm::vec3& diffuse = glm::vec3(0.8f),
            const glm::vec3& specular = glm::vec3(1.0f))
            : Direction(glm::normalize(glm::vec4(direction, 0.0f))),
            Ambient(glm::vec4(ambient, 0.0f)),
            Diffuse(glm::vec4(diffuse, 0.0f)),
            Specular(glm::vec4(specular, 0.0f))
        { }

        DirectionalLight(
            const glm::vec3& direction,
            int aR, int aG, int aB,
            int dR, int dG, int dB,
            int sR = 255, int sG = 255, int sB = 255)
            : Direction(glm::normalize(glm::vec4(direction, 0.0f)))
        {
            SetAmbientRGB(aR, aG, aB);
            SetDiffuseRGB(dR, dG, dB);
            SetSpecularRGB(sR, sG, sB);
        }

        void SetAmbientRGB(int r, int g, int b) 
        {
            Ambient = glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, 0.0f);
        }
        void SetDiffuseRGB(int r, int g, int b) 
        {
            Diffuse = glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, 0.0f);
        }
        void SetSpecularRGB(int r, int g, int b) 
        {
            Specular = glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, 0.0f);
        }

        glm::vec4 Direction = glm::normalize(glm::vec4(0.0f, -1.0f, 0.0f, 0.0f));

        glm::vec4 Ambient = glm::vec4(0.05f, 0.05f, 0.05f, 0.0f);
        glm::vec4 Diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 0.0f);
        glm::vec4 Specular = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
    };

    struct alignas(16) PointLight
    {
        PointLight() = default;

        PointLight(
            const glm::vec3& position,
            float constant = 1.0f,
            float linear = 0.09f,
            float quadratic = 0.032f,
            const glm::vec3& ambient = glm::vec3(0.05f),
            const glm::vec3& diffuse = glm::vec3(0.8f),
            const glm::vec3& specular = glm::vec3(1.0f))
            : Position(glm::vec4(position, 0.0f)),
            Constant(constant),
            Linear(linear),
            Quadratic(quadratic),
            Ambient(glm::vec4(ambient, 0.0f)),
            Diffuse(glm::vec4(diffuse, 0.0f)),
            Specular(glm::vec4(specular, 0.0f))
        { }

        PointLight(
            const glm::vec3& position,
            float constant,
            float linear,
            float quadratic,
            int aR, int aG, int aB,
            int dR, int dG, int dB,
            int sR = 255, int sG = 255, int sB = 255)
            : Position(glm::vec4(position, 0.0f)),
            Constant(constant), Linear(linear), Quadratic(quadratic)
        {
            SetAmbientRGB(aR, aG, aB);
            SetDiffuseRGB(dR, dG, dB);
            SetSpecularRGB(sR, sG, sB);
        }

        void SetAmbientRGB(int r, int g, int b) 
        {
            Ambient = glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, 0.0f);
        }
        void SetDiffuseRGB(int r, int g, int b) 
        {
            Diffuse = glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, 0.0f);
        }
        void SetSpecularRGB(int r, int g, int b) 
        {
            Specular = glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, 0.0f);
        }

        glm::vec4 Position = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

        float Constant = 1.0f;
        float Linear = 0.09f;
        float Quadratic = 0.032f;

        glm::vec4 Ambient = glm::vec4(0.05f, 0.05f, 0.05f, 0.0f);
        glm::vec4 Diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 0.0f);
        glm::vec4 Specular = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
    };

    struct alignas(16) SpotLight
    {
        SpotLight() = default;

        SpotLight(
            const glm::vec3& position,
            const glm::vec3& direction,
            float innerCutDeg = 12.5f,
            float outerCutDeg = 15.0f,
            float constant = 1.0f,
            float linear = 0.09f,
            float quadratic = 0.032f,
            const glm::vec3& ambient = glm::vec3(0.0f),
            const glm::vec3& diffuse = glm::vec3(1.0f),
            const glm::vec3& specular = glm::vec3(1.0f))
            : Position(glm::vec4(position, 0.0f)),
            Direction(glm::normalize(glm::vec4(direction, 0.0f))),
            CutOff(glm::cos(glm::radians(innerCutDeg))),
            OuterCutOff(glm::cos(glm::radians(outerCutDeg))),
            Constant(constant),
            Linear(linear),
            Quadratic(quadratic),
            Ambient(glm::vec4(ambient, 0.0f)),
            Diffuse(glm::vec4(diffuse, 0.0f)),
            Specular(glm::vec4(specular, 0.0f))
        { }

        SpotLight(
            const glm::vec3& position,
            const glm::vec3& direction,
            float innerCutDeg,
            float outerCutDeg,
            float constant,
            float linear,
            float quadratic,
            int aR, int aG, int aB,
            int dR, int dG, int dB,
            int sR = 255, int sG = 255, int sB = 255)
            : Position(glm::vec4(position, 0.0f)),
            Direction(glm::normalize(glm::vec4(direction, 0.0f))),
            CutOff(glm::cos(glm::radians(innerCutDeg))),
            OuterCutOff(glm::cos(glm::radians(outerCutDeg))),
            Constant(constant), Linear(linear), Quadratic(quadratic)
        {
            SetAmbientRGB(aR, aG, aB);
            SetDiffuseRGB(dR, dG, dB);
            SetSpecularRGB(sR, sG, sB);
        }

        void SetAmbientRGB(int r, int g, int b) 
        {
            Ambient = glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, 0.0f);
        }
        void SetDiffuseRGB(int r, int g, int b) 
        {
            Diffuse = glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, 0.0f);
        }
        void SetSpecularRGB(int r, int g, int b) 
        {
            Specular = glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, 0.0f);
        }

        glm::vec4 Position = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
        glm::vec4 Direction = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);

        float CutOff = glm::cos(glm::radians(12.5f));
        float OuterCutOff = glm::cos(glm::radians(15.0f));
        float Constant = 1.0f;
        float Linear = 0.09f;
        float Quadratic = 0.032f;

        glm::vec4 Ambient = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
        glm::vec4 Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
        glm::vec4 Specular = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
    };
}