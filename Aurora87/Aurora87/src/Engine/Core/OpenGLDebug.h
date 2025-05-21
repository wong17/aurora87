#pragma once

#include <glad/glad.h>

#include <iostream>
#include <string>

namespace Engine
{
	class OpenGLDebug
	{
	public:
		static void Enable() 
		{
			if (!GLAD_GL_VERSION_4_3)
			{
				std::cerr << "[OpenGL Warning] GL_KHR_debug no está disponible. "
					<< "No se habilitará OpenGL debug callback." << std::endl;
			}

			// Sólo disponible a partir de OpenGL 4.3 / ARB_debug_output
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(MessageCallback, nullptr);
			// para filtrar notificaciones muy verbosas
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
		}

	private:
		static const char* _Source(GLenum src) 
		{
			switch (src) 
			{
				case GL_DEBUG_SOURCE_API:             return "API";
				case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "Window Sys";
				case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader";
				case GL_DEBUG_SOURCE_THIRD_PARTY:     return "Third Party";
				case GL_DEBUG_SOURCE_APPLICATION:     return "App";
				case GL_DEBUG_SOURCE_OTHER:           return "Other";
				default:                              return "Unknown";
			}
		}
		static const char* _Type(GLenum type) 
		{
			switch (type) 
			{
				case GL_DEBUG_TYPE_ERROR:               return "Error";
				case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated";
				case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "Undefined";
				case GL_DEBUG_TYPE_PORTABILITY:         return "Portability";
				case GL_DEBUG_TYPE_PERFORMANCE:         return "Performance";
				case GL_DEBUG_TYPE_MARKER:              return "Marker";
				case GL_DEBUG_TYPE_PUSH_GROUP:          return "PushGrp";
				case GL_DEBUG_TYPE_POP_GROUP:           return "PopGrp";
				case GL_DEBUG_TYPE_OTHER:               return "Other";
				default:                                return "Unknown";
			}
		}
		static const char* _Severity(GLenum sev) 
		{
			switch (sev) 
			{
				case GL_DEBUG_SEVERITY_HIGH:         return "HIGH";
				case GL_DEBUG_SEVERITY_MEDIUM:       return "MEDIUM";
				case GL_DEBUG_SEVERITY_LOW:          return "LOW";
				case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFY";
				default:                             return "Unknown";
			}
		}

		static void APIENTRY MessageCallback(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam)
		{
			std::cerr
				<< "[OpenGL Debug] (" << id << "): " << message << "\n"
				<< "  Source:   " << _Source(source) << "\n"
				<< "  Type:     " << _Type(type) << "\n"
				<< "  Severity: " << _Severity(severity) << "\n"
				<< std::endl;
		}
	};
}