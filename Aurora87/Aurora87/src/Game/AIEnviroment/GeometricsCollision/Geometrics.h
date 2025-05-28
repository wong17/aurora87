#pragma once
#include <array>
#include <glm/glm.hpp>
#include <numeric>
#include "Engine/OpenGL/AssimpTypes.h"
#include <Engine/OpenGL/Model.h>
namespace Collision {
	struct Plane {
		glm::vec3 m_point;
		glm::vec3 m_normal;

		Plane() = default;

		Plane(const glm::vec3& a, const glm::vec3& b, const glm::vec3 c) noexcept
			: m_point(a), m_normal{ glm::normalize(glm::cross(b - a,c - a)) } {
		}
	};
	struct AABBXZ {
		using vec3 = glm::vec3;

		float m_min_x;
		float m_max_x;
		float m_min_z;
		float m_max_z;
		AABBXZ(float min_x, float max_x, float min_z, float  max_z) noexcept
			: m_min_x(min_x), m_max_x(max_x), m_min_z(min_z), m_max_z(max_z) {
		}
		void translate(const vec3& translation) noexcept {
			m_min_x += translation.x;
			m_max_x += translation.x;
			m_min_z += translation.z;
			m_max_z += translation.z;
		}

	};
	struct AABB {
		using vec3 = glm::vec3;
		vec3 m_min;
		vec3 m_max;
		AABB(const vec3& min, const vec3& max) noexcept
			: m_min(min), m_max(max) {
		}
		void translate(const vec3& translation) noexcept {
			m_min += translation;
			m_max += translation;
		}
	};
	struct AABBPLANES {
		Plane m_planes[6];

		Plane& north() noexcept { return m_planes[0]; }
		Plane& south() noexcept { return m_planes[1]; }
		Plane& east() noexcept { return m_planes[2]; }
		Plane& west() noexcept { return m_planes[3]; }
		Plane& top() noexcept { return m_planes[4]; }
		Plane& bottom() noexcept { return m_planes[5]; }

		explicit AABBPLANES(AABB const& aabb) {
			glm::vec3 x = glm::vec3(aabb.m_min.x, aabb.m_max.y, aabb.m_min.z);
			glm::vec3 y = glm::vec3(aabb.m_max.x, aabb.m_min.y, aabb.m_max.z);
			glm::vec3 z = glm::vec3(aabb.m_max.x, aabb.m_max.y, aabb.m_min.z);

			glm::vec3 xy = glm::vec3(aabb.m_min.x, aabb.m_min.y, aabb.m_max.z);
			glm::vec3 xz = glm::vec3(aabb.m_min.x, aabb.m_max.y, aabb.m_min.z);
			glm::vec3 yz = glm::vec3(aabb.m_max.x, aabb.m_min.y, aabb.m_min.z);

			glm::vec3 xyz = glm::vec3(aabb.m_min.x, aabb.m_min.y, aabb.m_min.z);

			north() = { xyz, yz, z };
			south() = { xy, aabb.m_max, y };
			east() = { yz, y , aabb.m_max };
			west() = { xyz, x, xy };
			top() = { xz, aabb.m_max, x };
			bottom() = { xyz, xy, y };
		}
	};
	class GeometricsBox
	{
	public:
		AABBXZ Calculate_AABBXZ(std::shared_ptr<Engine::Mesh> vertexModel)noexcept;
		AABB Calculate_AABB(std::shared_ptr<Engine::Mesh> vertexModel)noexcept;
	};


}