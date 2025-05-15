#pragma once

#include <glad/glad.h>
#include <iostream>

namespace Engine
{
	// limpia el queue de errores
	inline void GL_ClearErrors() {
		while (glGetError() != GL_NO_ERROR);
	}

	// chequea y reporta errores con linea y archivo
	inline bool GL_LogCall(const char* func, const char* file, int line) {
		bool ok = true;
		while (GLenum err = glGetError()) {
			std::cerr
				<< "[OpenGL Error] (" << err << "): " << func << " in " << file 
				<< ":" << line << std::endl;
			ok = false;
		}
		return ok;
	}

	// es para envolver las llamadas a OpenGL y reportar errores
#define GLCall(x) \
    do { \
        GL_ClearErrors(); \
        x; \
        GL_LogCall(#x, __FILE__, __LINE__); \
    } while(0)
}