#pragma once

#include "Shader.h"
#include "Engine/Core/DebugGL.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>
#include <string>
#include <iostream>
#include <stdexcept>

namespace Engine
{
	// Stores all state information relevant to a single character glyph
	struct Character {
		GLuint     TextureID;  // ID handle of the glyph texture
		glm::ivec2 Size;       // Size of glyph
		glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
		GLuint     Advance;    // Offset to advance to next glyph (in 1/64 pixels)
	};

	class TextRenderer {
	public:
		TextRenderer(const std::string& fontPath, GLuint fontSize);
		~TextRenderer();

		void RenderText(Shader& shader, const std::string& text, GLfloat x, GLfloat y, GLfloat scale, const glm::vec3& color);

	private:
		void LoadCharacters();
		void SetupRenderData();
	private:
		std::map<GLchar, Character> m_Characters;
		GLuint m_VertexArray, m_VertexBuffer;
		FT_Library m_FT;
		FT_Face m_Face;
		std::string m_FontPath;
		GLuint m_FontSize;
	};
}