#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <string>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include <random>
#include <vector>
#include <numbers>
#include <filesystem>

namespace Engine
{
	class Utils
	{
	public:
		// Function to extract the file name
		static inline std::string ExtractFileName(const std::string& path)
		{
			// Search for the last ‘/’ or ‘/’ (system dependent)
			size_t lastSlash = path.find_last_of("/\\");
			std::string filename = (lastSlash != std::string::npos) ? path.substr(lastSlash + 1) : path;

			// Delete the extension (look for the last ‘.’)
			size_t dotPos = filename.find_last_of('.');
			if (dotPos != std::string::npos)
				filename = filename.substr(0, dotPos);

			return filename;
		}

		/*
		 * This calculates the mipmap levels that a texture can have,
		 * log2(max(width, height)) + 1.
		 */
		static inline GLsizei CalculateMipLevels(uint32_t width, uint32_t height) {
			return static_cast<GLsizei>(std::floor(std::log2(std::max(width, height)))) + 1;
		}

		// Generates random transformation matrices to randomly place elements around the scene
		static inline std::vector<std::vector<glm::mat4>> GenerateTransformBatches(
				unsigned int batches,
				unsigned int instances,
				const glm::vec3& areaMin,
				const glm::vec3& areaMax,
				float minScale = 1.0f,
				float maxScale = 5.0f)
		{
			std::vector<std::vector<glm::mat4>> all;
			all.reserve(batches);

			for (unsigned int b = 0; b < batches; ++b)
				all.push_back(GenerateTransformMatrices(instances, areaMin, areaMax, minScale, maxScale));
			
			return all;
		}

		// Generates random transformation matrices to randomly place elements around the scene
		static inline std::vector<glm::mat4> GenerateTransformMatrices(
			unsigned int instances,
			const glm::vec3& areaMin,
			const glm::vec3& areaMax,
			float minScale = 1.0f,
			float maxScale = 5.0f)
		{
			std::vector<glm::mat4> transformations;
			transformations.reserve(instances);

			// Engine and distributions
			static std::random_device rd;
			static std::mt19937 gen(rd());
			std::uniform_real_distribution<float> distX(areaMin.x, areaMax.x);
			std::uniform_real_distribution<float> distY(areaMin.y, areaMax.y);
			std::uniform_real_distribution<float> distZ(areaMin.z, areaMax.z);

			std::uniform_real_distribution<float> distRot(0.0f, glm::two_pi<float>());
			std::uniform_real_distribution<float> distScale(minScale, maxScale);

			for (unsigned int i = 0; i < instances; ++i)
			{
				// moves them randomly, always within the specified area between areaMin and areaMax
				glm::vec3 pos{ distX(gen), distY(gen), distZ(gen) };

				// rotates them randomly
				float rotX = distRot(gen);
				float rotY = distRot(gen);
				float rotZ = distRot(gen);

				// scales them uniformly and randomly
				float s = distScale(gen);

				// we create the model matrix by multiplying the transformations: M = T * R * S
				glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
				model = glm::rotate(model, rotX, glm::vec3(1, 0, 0));
				model = glm::rotate(model, rotY, glm::vec3(0, 1, 0));
				model = glm::rotate(model, rotZ, glm::vec3(0, 0, 1));
				model = glm::scale(model, glm::vec3(s));

				transformations.push_back(model);
			}

			return transformations;
		}

		/**
		 * Generates an extruded polygon (a prismatic cylinder) centered at (cx,cy),
		 * with numSides, radius and thickness.
		 *
		 * Instead of using indexes, dump directly into the VBO all vertices
		 * of each triangle, in the correct order to draw them with glDrawArrays.
		 *
		 * @param vertices Target vector of floats: x,y,z,u,v
		 * @param cx, cy Center in XY
		 * @param numSides Number of sides (≥3)
		 * @param radius Radius in XY
		 * @param thickness Total thickness in Z (extrudes ± thickness/2)
		 */
		static void GenerateExtrudedPolygonNoIndices(
			std::vector<float>& vertices,
			float cx, float cy,
			int numSides, float radius,
			float thickness)
		{
			if (numSides < 3 || radius <= 0.0f || thickness <= 0.0f)
				return;

			float h = thickness * 0.5f;

			// Precompute ring positions + two sets of UVs
			std::vector<glm::vec3> botPos(numSides), topPos(numSides);
			std::vector<glm::vec2> diskUV(numSides), sideU(numSides);
			for (int i = 0; i < numSides; ++i) {
				float θ = static_cast<float>((2 * std::numbers::pi * i) / numSides);
				float c = std::cos(θ), s = std::sin(θ);
				botPos[i] = { cx + radius * c, cy + radius * s, -h };
				topPos[i] = { cx + radius * c, cy + radius * s, +h };
				diskUV[i] = { c * 0.5f + 0.5f, s * 0.5f + 0.5f };
				sideU[i] = { float(i) / numSides, 0.0f }; // v we will then fill in
			}
			glm::vec3 centerB{ cx,cy,-h }, centerT{ cx,cy,+h };
			glm::vec2 centerUV{ 0.5f,0.5f };

			auto push = [&](const glm::vec3 P, const glm::vec3& N, const glm::vec2 UV) {
				vertices.push_back(P.x);
				vertices.push_back(P.y);
				vertices.push_back(P.z);
				vertices.push_back(N.x);
				vertices.push_back(N.y);
				vertices.push_back(N.z);
				vertices.push_back(UV.x);
				vertices.push_back(UV.y);
				};

			// 1) Bottom cover (bottom oriented)
			{
				glm::vec3 N = { 0, 0, -1 };
				for (int i = 0; i < numSides; ++i) {
					int n = (i + 1) % numSides;
					push(centerB, N, centerUV);
					push(botPos[n], N, diskUV[n]);
					push(botPos[i], N, diskUV[i]);
				}
			}
			// 2) Top cover
			{
				glm::vec3 N = { 0, 0, +1 };
				for (int i = 0; i < numSides; ++i) {
					int n = (i + 1) % numSides;
					push(centerT, N, centerUV);
					push(topPos[i], N, diskUV[i]);
					push(topPos[n], N, diskUV[n]);
				}
			}
			// 3) Side faces
			for (int i = 0; i < numSides; ++i) {
				int n = (i + 1) % numSides;
				// to calculate the normal
				glm::vec3 edge1 = botPos[n] - botPos[i];
				glm::vec3 edge2 = topPos[i] - botPos[i];
				glm::vec3 N = glm::normalize(glm::cross(edge1, edge2));
				// Triangle A: bot i, bot n, top n
				push(botPos[i], N, { sideU[i].x, 0.0f });
				push(botPos[n], N, { sideU[n].x, 0.0f });
				push(topPos[n], N, { sideU[n].x, 1.0f });

				// Triangle B: bot i, top n, top i
				push(botPos[i], N, { sideU[i].x, 0.0f });
				push(topPos[n], N, { sideU[n].x, 1.0f });
				push(topPos[i], N, { sideU[i].x, 1.0f });
			}
		}
	};
}