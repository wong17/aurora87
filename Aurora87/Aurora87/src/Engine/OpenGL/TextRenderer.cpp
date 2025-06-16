#include "TextRenderer.h"

namespace Engine
{
	TextRenderer::TextRenderer(const std::string& fontPath, GLuint fontSize)
		: m_FontPath(fontPath), m_FontSize(fontSize), m_VertexArray(0), m_VertexBuffer(0), m_FT(nullptr), m_Face(nullptr)
	{
		// Initialize FreeType library
		if (FT_Init_FreeType(&m_FT)) {
            throw std::runtime_error("TextRenderer::FREETYPE: Could not init FreeType Library");
			return;
		}
		// Load font face
		if (FT_New_Face(m_FT, m_FontPath.c_str(), 0, &m_Face)) {
			throw std::runtime_error("TextRenderer::FREETYPE: Failed to load font");
			return;
		}
		// Set font size
		FT_Set_Pixel_Sizes(m_Face, 0, m_FontSize);

		// Load glyphs and setup OpenGL buffers
		LoadCharacters();
		SetupRenderData();
	}

	TextRenderer::~TextRenderer()
	{
		FT_Done_Face(m_Face);
		FT_Done_FreeType(m_FT);
		glDeleteVertexArrays(1, &m_VertexArray);
		glDeleteBuffers(1, &m_VertexBuffer);
	}

	void TextRenderer::RenderText(Shader& shader, const std::string& text, GLfloat x, GLfloat y, GLfloat scale, const glm::vec3& color)
	{
        // Activate corresponding render state
        shader.Bind();
        shader.SetFloat3("u_TextColor", color);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(m_VertexArray);

        // Iterate through characters
        for (const char c : text) {
            Character ch = m_Characters[c];
            GLfloat xpos = x + ch.Bearing.x * scale;
            GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
            GLfloat w = ch.Size.x * scale;
            GLfloat h = ch.Size.y * scale;
            // Update VBO for each character
            GLfloat vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
            };
            // Render glyph texture over quad
            glBindTextureUnit(0, ch.TextureID);
            GLCall(glNamedBufferSubData(m_VertexBuffer, 0, sizeof(vertices), vertices));
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // Advance cursors for next glyph
            x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
	}

	void TextRenderer::LoadCharacters()
	{
        // Load first 128 ASCII characters
        GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1)); // Disable byte-alignment restriction
        for (GLubyte c = 0; c < 128; ++c) {
            // Load glyph
            if (FT_Load_Char(m_Face, c, FT_LOAD_RENDER)) {
                std::cerr << "TextRenderer::FREETYPE: Failed to load Glyph " << c << std::endl;
                continue;
            }
            // Generate texture
            int width = m_Face->glyph->bitmap.width;
            int height = m_Face->glyph->bitmap.rows;
            GLuint texture;

            // If the glyph has no bitmap (space, etc.), we create a 1x1 texture.
            unsigned char zero = 0;
            glCreateTextures(GL_TEXTURE_2D, 1, &texture);
            GLCall(glTextureStorage2D(texture, 1, GL_R8, width <= 0 ? 1 : width, height <= 0 ? 1 : height));
            GLCall(glTextureSubImage2D(texture, 0, 0, 0, width <= 0 ? 1 : width, height <= 0 ? 1 : height, 
                GL_RED, GL_UNSIGNED_BYTE, (width <= 0 || height <= 0) ? &zero : m_Face->glyph->bitmap.buffer));

            // Texture options
            glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Store character
            Character character = {
                texture,
                glm::ivec2(width <= 0 ? 1 : width, height <= 0 ? 1 : height),
                glm::ivec2(m_Face->glyph->bitmap_left, m_Face->glyph->bitmap_top),
                static_cast<GLuint>(m_Face->glyph->advance.x)
            };
            m_Characters.emplace(c, character);
        }
        glBindTexture(GL_TEXTURE_2D, 0);
	}

	void TextRenderer::SetupRenderData()
	{
        // Configure VAO/VBO for texture quads
        glCreateVertexArrays(1, &m_VertexArray);
        glCreateBuffers(1, &m_VertexBuffer);
        GLCall(glNamedBufferStorage(m_VertexBuffer, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_STORAGE_BIT));
        // 6 vertices, 4 floats each (x, y, u, v)
        GLCall(glVertexArrayVertexBuffer(m_VertexArray, 0, m_VertexBuffer, 0, sizeof(GLfloat) * 4));
        glEnableVertexArrayAttrib(m_VertexArray, 0);
        GLCall(glVertexArrayAttribFormat(m_VertexArray, 0, 4, GL_FLOAT, GL_FALSE, 0));
        glVertexArrayAttribBinding(m_VertexArray, 0, 0);
	}
}