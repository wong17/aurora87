#pragma once

#include <glad/glad.h>
#include <iostream>

namespace Engine
{
	// Symbolic translation of GL errors
	inline const char* GetGLErrorString(GLenum error)
	{
		switch (error)
		{
			case GL_NO_ERROR:                      return "GL_NO_ERROR";
			case GL_INVALID_ENUM:                  return "GL_INVALID_ENUM";
			case GL_INVALID_VALUE:                 return "GL_INVALID_VALUE";
			case GL_INVALID_OPERATION:             return "GL_INVALID_OPERATION";
			case GL_STACK_OVERFLOW:                return "GL_STACK_OVERFLOW";
			case GL_STACK_UNDERFLOW:               return "GL_STACK_UNDERFLOW";
			case GL_OUT_OF_MEMORY:                 return "GL_OUT_OF_MEMORY";
			case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
			default:                               return "UNKNOWN_ERROR";
		}
	}

	// Symbolic translation of framebuffer states
	inline const char* FramebufferStatusToString(GLenum status)
	{
		switch (status)
		{
			case GL_FRAMEBUFFER_COMPLETE:						return "GL_FRAMEBUFFER_COMPLETE";
			case GL_FRAMEBUFFER_UNDEFINED:						return "GL_FRAMEBUFFER_UNDEFINED";
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:			return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:	return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:			return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:			return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
			case GL_FRAMEBUFFER_UNSUPPORTED:					return "GL_FRAMEBUFFER_UNSUPPORTED";
			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:			return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:		return "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
			default:											return "UNKNOWN_FRAMEBUFFER_STATUS";
		}
	}

	// clears the queue of errors
	inline void GL_ClearErrors() 
	{
		while (glGetError() != GL_NO_ERROR);
	}

	// checks and reports line and file errors
	inline bool GL_LogCall(const char* func, const char* file, int line) 
	{
		bool ok = true;
		while (GLenum err = glGetError()) {
			std::cerr
				<< "[OpenGL Error] (" << err << "): " << func << " in " << file
				<< ":" << line << std::endl;
			ok = false;
		}
		return ok;
	}

	// is for wrapping OpenGL calls and reporting errors
#define GLCall(x) \
    do { \
        GL_ClearErrors(); \
        x; \
        GL_LogCall(#x, __FILE__, __LINE__); \
    } while(0)
}