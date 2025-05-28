#include "UI/Button3D.h"
#include <GLFW/glfw3.h> 
#include <iostream>

Button3D::Button3D(std::shared_ptr<Engine::Entity> entity,
    std::shared_ptr<Engine::Texture> normalTex,
    std::shared_ptr<Engine::Texture> hoverTex)
    : m_Entity(entity), m_TextureNormal(normalTex), m_TextureHover(hoverTex),
    m_MousePressedLastFrame(false), m_IsMouseOver(false), m_HoverCounter(1), m_OnClick(nullptr)
{
}

void Button3D::Update(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos, GLFWwindow* window)
{
    // Obtiene la posicion del mouse
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    int winWidth, winHeight;
    glfwGetWindowSize(window, &winWidth, &winHeight);

    // Convierte la posicion del mouse a coordenadas NDC [-1,1]
    float x = (2.0f * static_cast<float>(mouseX)) / winWidth - 1.0f;
    float y = 1.0f - (2.0f * static_cast<float>(mouseY)) / winHeight;
    glm::vec4 ray_clip(x, y, -1.0f, 1.0f);

    // Transforma el rayo a espacio ojo
    glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);

    // Transforma el rayo a espacio mundo
    glm::vec3 ray_world = glm::normalize(glm::vec3(glm::inverse(view) * ray_eye));

    // Obtiene el modelo y normal del plano (Button)
    auto& transform = m_Entity->GetTransformComponent();
    glm::mat4 model = transform.GetLocalMatrix();

    glm::vec3 planeOrigin = glm::vec3(model * glm::vec4(0, 0, 0, 1));
    glm::vec3 planeNormal = glm::normalize(glm::vec3(model * glm::vec4(0, 1, 0, 0)));

    // Interseccion rayo-plano
    float denom = glm::dot(planeNormal, ray_world);
    bool hit = false;
    glm::vec3 hitPoint;

    if (std::abs(denom) > 1e-6f) // Evita el paralelismo
    {
        float t = glm::dot(planeOrigin - cameraPos, planeNormal) / denom;
        if (t >= 0.0f)
        {
            hitPoint = cameraPos + t * ray_world;

            // Transforma el punto de intersección a espacio local del boton
            glm::mat4 invModel = glm::inverse(model);
            glm::vec3 localPoint = glm::vec3(invModel * glm::vec4(hitPoint, 1.0f));

            // Comprueba si el punto esta dentro del area del boton
            if (localPoint.x >= -0.5f && localPoint.x <= 0.5f &&
                localPoint.z >= -0.5f && localPoint.z <= 0.5f)
            {
                hit = true;
            }
        }
    }

    m_IsMouseOver = hit;

    // Detecta el estado del mouse
    bool mousePressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    // Detecta el click (solo en el frame que se presiona)
    if (mousePressed && !m_MousePressedLastFrame && hit)
    {
        std::cout << "El boton ha sido presionado." << std::endl;
        if (m_OnClick)
            m_OnClick();
    }

    m_MousePressedLastFrame = mousePressed;

    // Cambia textura Normal/Hover
    if (hit)
    {
        std::cout << "El cursor esta encima del boton." << m_HoverCounter++ << std::endl;
        m_Entity->GetMesh()->SetTexture(m_TextureHover);
    }
    else
    {
        m_Entity->GetMesh()->SetTexture(m_TextureNormal);
    }
}