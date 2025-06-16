#include "UI/Button3D.h"

namespace UI
{
	Button3D::Button3D(std::shared_ptr<Engine::Entity> entity,
		std::shared_ptr<Engine::Texture> normalTex,
		std::shared_ptr<Engine::Texture> hoverTex)
		: m_Entity(entity)
	{
		if (normalTex == nullptr || hoverTex == nullptr)
		{
			throw std::invalid_argument("Button3D::Button3D: Textures cannot be null");
		}

		auto mtt = Engine::MaterialTextureType::Diffuse;
		m_NormalTextureData = Engine::TextureData(mtt, normalTex, Engine::MaterialTextureUniformName(mtt), normalTex->GetSpecification().SRGB);
		m_HoverTextureData = Engine::TextureData(mtt, hoverTex, Engine::MaterialTextureUniformName(mtt), normalTex->GetSpecification().SRGB);
	}

	void Button3D::Update(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos, GLFWwindow* window)
	{
		// Gets the mouse position
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		int winWidth, winHeight;
		glfwGetWindowSize(window, &winWidth, &winHeight);

		// Convert mouse position to NDC coordinates [-1,1].
		float x = (2.0f * static_cast<float>(mouseX)) / winWidth - 1.0f;
		float y = 1.0f - (2.0f * static_cast<float>(mouseY)) / winHeight;
		glm::vec4 ray_clip(x, y, -1.0f, 1.0f);

		// Transforms lightning to eye space
		glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
		ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);

		// Transforms lightning to world space
		glm::vec3 ray_world = glm::normalize(glm::vec3(glm::inverse(view) * ray_eye));

		// Gets the model and plane normal (Button)
		auto& transform = m_Entity->GetTransformComponent();
		glm::mat4 model = transform.GetLocalMatrix();

		glm::vec3 planeOrigin = glm::vec3(model * glm::vec4(0, 0, 0, 1));
		glm::vec3 planeNormal = glm::normalize(glm::vec3(model * glm::vec4(0, 1, 0, 0)));

		// Ray-plane intersection
		float denom = glm::dot(planeNormal, ray_world);
		bool hit = false;
		glm::vec3 hitPoint;

		if (std::abs(denom) > 1e-6f) // Avoid parallelism
		{
			float t = glm::dot(planeOrigin - cameraPos, planeNormal) / denom;
			if (t >= 0.0f)
			{
				hitPoint = cameraPos + t * ray_world;

				// Transform the intersection point to local button space.
				glm::mat4 invModel = glm::inverse(model);
				glm::vec3 localPoint = glm::vec3(invModel * glm::vec4(hitPoint, 1.0f));

				// Check if the dot is inside the button area.
				if (localPoint.x >= -0.5f && localPoint.x <= 0.5f &&
					localPoint.z >= -0.5f && localPoint.z <= 0.5f)
				{
					hit = true;
				}
			}
		}

		m_IsMouseOver = hit;

		// Detects mouse status
		bool mousePressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

		// Click detection (only on the frame that is pressed)
		if (mousePressed && !m_MousePressedLastFrame && hit)
		{
			std::cout << "Button3D::Update: Button has been pressed." << std::endl;
			if (m_OnClick)
				m_OnClick();
		}

		m_MousePressedLastFrame = mousePressed;

		// Change texture Normal/Hover
		if (hit)
		{
			std::cout << "Button3D::Update: The cursor is over the button." << m_HoverCounter++ << std::endl;
			m_Entity->GetMesh()->SetTextures({ m_HoverTextureData }, true);
		}
		else
		{
			m_Entity->GetMesh()->SetTextures({ m_NormalTextureData }, true);
		}
	}
}