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
		// Función para extraer el nombre del archivo
		static inline std::string ExtractFileName(const std::string& path)
		{
			// Buscar el último '/' o '\' (depende del sistema)
			size_t lastSlash = path.find_last_of("/\\");
			std::string filename = (lastSlash != std::string::npos) ? path.substr(lastSlash + 1) : path;

			// Eliminar la extensión (buscamos el último '.')
			size_t dotPos = filename.find_last_of('.');
			if (dotPos != std::string::npos)
				filename = filename.substr(0, dotPos);

			return filename;
		}

		/*
		 * Esto calcula los niveles de mipmap que una textura puede tener,
		 * log2(max(width, height)) + 1.
		 */
		static inline GLsizei CalculateMipLevels(uint32_t width, uint32_t height) {
			return static_cast<GLsizei>(std::floor(std::log2(std::max(width, height)))) + 1;
		}

		// Genera matrices de transformación aleatorias para situar elementos de forma random por la escena
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

		// Genera matrices de transformación aleatorias para situar elementos de forma random por la escena
		static inline std::vector<glm::mat4> GenerateTransformMatrices(
			unsigned int instances,
			const glm::vec3& areaMin,
			const glm::vec3& areaMax,
			float minScale = 1.0f,
			float maxScale = 5.0f)
		{
			std::vector<glm::mat4> transformations;
			transformations.reserve(instances);

			// Motor y distribuciones
			static std::random_device rd;
			static std::mt19937 gen(rd());
			std::uniform_real_distribution<float> distX(areaMin.x, areaMax.x);
			std::uniform_real_distribution<float> distY(areaMin.y, areaMax.y);
			std::uniform_real_distribution<float> distZ(areaMin.z, areaMax.z);

			std::uniform_real_distribution<float> distRot(0.0f, glm::two_pi<float>());
			std::uniform_real_distribution<float> distScale(minScale, maxScale);

			for (unsigned int i = 0; i < instances; ++i)
			{
				// los traslada de forma random, siempre dentro del area especificada entre areaMin y areaMax
				glm::vec3 pos{ distX(gen), distY(gen), distZ(gen) };

				// los rota de forma aleatoria
				float rotX = distRot(gen);
				float rotY = distRot(gen);
				float rotZ = distRot(gen);

				// los escala uniformemete de forma aleatoria
				float s = distScale(gen);

				// creamos la matriz del modelo multiplicando las transformaciones: M = T * R * S
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
		 * Genera un polígono extrudido (un cilindro prismático) centrado en (cx,cy),
		 * con numSides lados, radio 'radius' y espesor total 'thickness'.
		 *
		 * En lugar de usar índices, vuelca directamente en el VBO todos los vértices
		 * de cada triángulo, en el orden correcto para dibujarlos con glDrawArrays.
		 *
		 * @param vertices  Vector destino de floats: x,y,z,u,v
		 * @param cx, cy    Centro en XY
		 * @param numSides  Número de lados (≥3)
		 * @param radius    Radio en XY
		 * @param thickness Grosor total en Z (se extruye ±thickness/2)
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
				sideU[i] = { float(i) / numSides, 0.0f }; // v rellenaremos luego
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

			// 1) Tapa inferior (orientada abajo)
			{
				glm::vec3 N = { 0, 0, -1 };
				for (int i = 0; i < numSides; ++i) {
					int n = (i + 1) % numSides;
					push(centerB, N, centerUV);
					push(botPos[n], N, diskUV[n]);
					push(botPos[i], N, diskUV[i]);
				}
			}
			// 2) Tapa superior
			{
				glm::vec3 N = { 0, 0, +1 };
				for (int i = 0; i < numSides; ++i) {
					int n = (i + 1) % numSides;
					push(centerT, N, centerUV);
					push(topPos[i], N, diskUV[i]);
					push(topPos[n], N, diskUV[n]);
				}
			}
			// 3) Caras laterales
			for (int i = 0; i < numSides; ++i) {
				int n = (i + 1) % numSides;
				// para calcular la normal
				glm::vec3 edge1 = botPos[n] - botPos[i];
				glm::vec3 edge2 = topPos[i] - botPos[i];
				glm::vec3 N = glm::normalize(glm::cross(edge1, edge2));
				// Triángulo A: bot i, bot n, top n
				push(botPos[i], N, { sideU[i].x, 0.0f });
				push(botPos[n], N, { sideU[n].x, 0.0f });
				push(topPos[n], N, { sideU[n].x, 1.0f });

				// Triángulo B: bot i, top n, top i
				push(botPos[i], N, { sideU[i].x, 0.0f });
				push(topPos[n], N, { sideU[n].x, 1.0f });
				push(topPos[i], N, { sideU[i].x, 1.0f });
			}
		}
	};
}