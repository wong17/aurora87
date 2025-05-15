#pragma once

#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glm/glm.hpp>

#include <array>
#include <string>

namespace Engine
{
	// Depth Test
	enum class DepthFunc {
		Never, Less, Equal, Lequal, Greater, NotEqual, Gequal, Always
	};

	// Cull Face
	enum class CullFaceMode {
		Front, Back, FrontAndBack
	};

	enum class FrontFace {
		CW, CCW
	};

	// Stencil Test
	enum class StencilFunc {
		Always, Never, Less, Equal, Lequal, Greater, NotEqual, Gequal
	};

	enum class StencilOp {
		Keep, Zero, Replace, Incr, Decr, IncrWrap, DecrWrap, Invert
	};

	// Alpha Blending
	enum class BlendFactor {
		Zero, One,
		SrcColor, OneMinusSrcColor,
		DstColor, OneMinusDstColor,
		SrcAlpha, OneMinusSrcAlpha,
		DstAlpha, OneMinusDstAlpha,
		ConstantColor, OneMinusConstantColor,
		ConstantAlpha, OneMinusConstantAlpha,
		SrcAlphaSaturate
	};

	enum class BlendEquation {
		Add, Subtract, ReverseSubtract, Min, Max
	};

	class RenderSettings
	{
	public:
		RenderSettings(Window& window) : m_Window(window) {}
		~RenderSettings() = default;

		// V-Sync
		void SetVSync(bool enabled) { m_VSync = enabled; }
		bool IsVSync() const { return m_VSync; }

		// Depth Test
		void SetDepthTest(bool enabled) 
		{
			m_DepthTest = enabled;
			if (enabled) 
				glEnable(GL_DEPTH_TEST);
			else 
				glDisable(GL_DEPTH_TEST);
		}
		void SetDepthFunc(DepthFunc func) 
		{ 
			m_DepthFunc = func;
			glDepthFunc(_toGLDepthFunc(func));
		}
		void SetDepthMask(bool enabled)
		{
			m_DepthMask = enabled;
			glDepthMask(enabled ? GL_TRUE : GL_FALSE);
		}
		bool IsDepthTest() const { return m_DepthTest; }
		DepthFunc GetDepthFunc() const { return m_DepthFunc; }
		bool IsDepthMask() const { return m_DepthMask; }

		// Cull Face
		void SetCullFace(bool enabled)
		{
			m_CullFace = enabled;
			if (enabled) 
				glEnable(GL_CULL_FACE);
			else 
				glDisable(GL_CULL_FACE);
		}
		void SetCullFaceMode(CullFaceMode mode) 
		{ 
			m_CullFaceMode = mode;
			if (m_CullFace) 
				glCullFace(_toGLCullFace(mode));
		}
		void SetFrontFace(FrontFace mode)
		{
			m_FrontFace = mode;
			glFrontFace(_toGLFrontFace(mode));
		}
		bool IsCullFace() const { return m_CullFace; }
		CullFaceMode GetCullFaceMode() const { return m_CullFaceMode; }
		FrontFace GetFrontFace() const { return m_FrontFace; }

		// Stencil Test
		void SetStencilTest(bool enabled) 
		{ 
			m_StencilTest = enabled;
			if (enabled) 
				glEnable(GL_STENCIL_TEST);
			else 
				glDisable(GL_STENCIL_TEST);
		}
		void SetStencilFunc(StencilFunc func, int ref, unsigned mask) 
		{ 
			m_StencilFunc = func;	// criterio de comparación
			m_StencilRef = ref;		// valor de referencia para realizar la comparación
			m_StencilMask = mask;	// bits activos de la comparación
			glStencilFunc(_toGLStencilFunc(func), ref, mask);
		}
		void SetStencilOp(StencilOp sfail, StencilOp depthfail, StencilOp depthpass) 
		{ 
			// Indica qué escribir en el stencil cuando un fragmento:
			m_StencilFail = sfail;			// no pasa la prueba de stencil
			m_StencilDepthFail = depthfail;	// pasa la prueba de stencil pero no la de profundidad
			m_StencilDepthPass = depthpass;	// pasa ambas pruebas
			glStencilOp(_toGLStencilOp(sfail), _toGLStencilOp(depthfail), _toGLStencilOp(depthpass));
		}
		void SetStencilWriteMask(unsigned mask) 
		{ 
			// Controla qué bits del stencil buffer pueden escribirse (es útil para bloquear o permitir escritura)
			m_StencilWriteMask = mask;
			glStencilMask(mask);
		}
		bool IsStencilTest() const { return m_StencilTest; }
		StencilFunc GetStencilFunc() const { return m_StencilFunc; }
		int GetStencilRef() const { return m_StencilRef; }
		unsigned int GetStencilMask() const { return m_StencilMask; }
		unsigned int GetStencilWriteMask() const { return m_StencilWriteMask; }
		StencilOp GetStencilFail() const { return m_StencilFail; }
		StencilOp GetStencilDepthFail() const { return m_StencilDepthFail; }
		StencilOp GetStencilDepthPass() const { return m_StencilDepthPass; }

		// Alpha Blending
		void SetAlphaBlending(bool enabled) 
		{ 
			m_AlphaBlending = enabled;
			if (enabled) 
				glEnable(GL_BLEND);
			else 
				glDisable(GL_BLEND);
		}
		void SetBlendEquation(BlendEquation equation)
		{
			m_BlendEquation = equation;
			glBlendEquation(_toGLBlendEquation(equation));
		}
		void SetBlendFactors(BlendFactor srcRGB, BlendFactor dstRGB)
		{
			m_BlendSrcRGB = srcRGB;
			m_BlendDstRGB = dstRGB;
			glBlendFunc(_toGLBlendFactor(srcRGB), _toGLBlendFactor(dstRGB));
		}
		void SetBlendFactors(BlendFactor srcRGB, BlendFactor dstRGB, BlendFactor srcAlpha, BlendFactor dstAlpha) {
			m_BlendSrcRGB = srcRGB; 
			m_BlendDstRGB = dstRGB;
			m_BlendSrcAlpha = srcAlpha;
			m_BlendDstAlpha = dstAlpha;
			glBlendFuncSeparate(_toGLBlendFactor(srcRGB), _toGLBlendFactor(dstRGB), _toGLBlendFactor(srcAlpha), _toGLBlendFactor(dstAlpha));
		}
		bool IsAlphaBlending() const { return m_AlphaBlending; }
		BlendFactor GetBlendSrcRGB() const { return m_BlendSrcRGB; }
		BlendFactor GetBlendDstRGB() const { return m_BlendDstRGB; }
		BlendFactor GetBlendSrcAlpha() const { return m_BlendSrcAlpha; }
		BlendFactor GetBlendDstAlpha() const { return m_BlendDstAlpha; }
		BlendEquation GetBlendEquation() const { return m_BlendEquation; }

		// Point size GLSL
		void SetPointSize(bool enabled)
		{
			m_PointSize = enabled;
			if (enabled)
				glEnable(GL_PROGRAM_POINT_SIZE);
			else
				glDisable(GL_PROGRAM_POINT_SIZE);
		}
		void SetPointSizeValue(float size) {
			m_PointSizeValue = size;
			glPointSize(size);
		}

		inline void OnImGuiRender() {
			if (!ImGui::CollapsingHeader("Render Settings")) {
				// Clear Color
				if (ImGui::Button("Pick Clear Color")) {
					ImGui::OpenPopup("ClearColorPicker");
				}
				if (ImGui::BeginPopup("ClearColorPicker")) {
					ImGui::Text("Select Clear Color");
					ImGui::Separator();
					ImGui::ColorPicker4("##picker", &m_ClearColor[0]);
					ImGui::EndPopup();
				}

				// V-Sync
				ImGui::Checkbox("V-Sync", &m_VSync);

				// Depth Test
				ImGui::Checkbox("Depth Test", &m_DepthTest);
				ImGui::SameLine(); 
				ImGui::Text("Func:");
				{
					static const char* names[] = 
					{
					  "Never","Less","Equal","Lequal","Greater","NotEqual","Gequal","Always"
					};
					int idx = static_cast<int>(m_DepthFunc);
					ImGui::Combo("##depthFunc", &idx, names, IM_ARRAYSIZE(names));
					m_DepthFunc = (DepthFunc)idx;
				}
				ImGui::Checkbox("Depth Mask", &m_DepthMask);

				// Cull Face
				ImGui::Checkbox("Cull Face", &m_CullFace);
				ImGui::SameLine(); 
				ImGui::Text("Mode:");
				{
					static const char* names[] = { "Front","Back","FrontAndBack" };
					int idx = static_cast<int>(m_CullFaceMode);
					ImGui::Combo("##cullMode", &idx, names, IM_ARRAYSIZE(names));
					m_CullFaceMode = (CullFaceMode)idx;
				}
				ImGui::Text("Front Face:");
				{
					static const char* frontFaceNames[] = { "CW", "CCW" };
					int frontFaceIdx = static_cast<int>(m_FrontFace);
					if (ImGui::Combo("##frontFace", &frontFaceIdx, frontFaceNames, IM_ARRAYSIZE(frontFaceNames))) {
						m_FrontFace = (FrontFace)frontFaceIdx;
						SetFrontFace(m_FrontFace);
					}
				}

				// Stencil Test
				ImGui::Checkbox("Stencil Test", &m_StencilTest);
				if (m_StencilTest) {
					ImGui::Indent();
					{
						static const char* funcNames[] = { "Always","Never","Less","Equal","Lequal","Greater","NotEqual","Gequal" };
						int idx = static_cast<int>(m_StencilFunc);
						ImGui::Combo("Func", &idx, funcNames, IM_ARRAYSIZE(funcNames));
						m_StencilFunc = (StencilFunc)idx;
					}
					ImGui::SliderInt("Ref", &m_StencilRef, 0, 255);
					ImGui::SliderInt("Mask", &m_StencilMask, 0, 0xFF);
					{
						static const char* opNames[] = { "Keep","Zero","Replace","Incr","Decr","IncrWrap","DecrWrap","Invert" };
						int idx = static_cast<int>(m_StencilFail);
						ImGui::Combo("Fail", &idx, opNames, IM_ARRAYSIZE(opNames));
						m_StencilFail = (StencilOp)idx;
						idx = static_cast<int>(m_StencilDepthFail);
						ImGui::Combo("DepthFail", &idx, opNames, IM_ARRAYSIZE(opNames));
						m_StencilDepthFail = (StencilOp)idx;
						idx = static_cast<int>(m_StencilDepthPass);
						ImGui::Combo("DepthPass", &idx, opNames, IM_ARRAYSIZE(opNames));
						m_StencilDepthPass = (StencilOp)idx;
					}
					ImGui::Unindent();
				}

				// Alpha Blending
				ImGui::Checkbox("Alpha Blending", &m_AlphaBlending);
				if (m_AlphaBlending) {
					ImGui::Indent();
					{
						static const char* bf[] = 
						{ 
							"Zero","One","SrcColor","1-SrcColor","DstColor","1-DstColor",
							"SrcAlpha","1-SrcAlpha","DstAlpha","1-DstAlpha","ConstColor",
							"1-ConstColor","ConstAlpha","1-ConstAlpha","SrcAlphaSat" 
						};
						int srcRGB = static_cast<int>(m_BlendSrcRGB), dstRGB = static_cast<int>(m_BlendDstRGB);
						ImGui::Combo("SrcRGB", &srcRGB, bf, IM_ARRAYSIZE(bf));
						ImGui::Combo("DstRGB", &dstRGB, bf, IM_ARRAYSIZE(bf));
						int srcA = static_cast<int>(m_BlendSrcAlpha), dstA = static_cast<int>(m_BlendDstAlpha);
						ImGui::Combo("SrcAlpha", &srcA, bf, IM_ARRAYSIZE(bf));
						ImGui::Combo("DstAlpha", &dstA, bf, IM_ARRAYSIZE(bf));
						SetBlendFactors((BlendFactor)srcRGB, (BlendFactor)dstRGB, (BlendFactor)srcA, (BlendFactor)dstA);
						
						static const char* eq[] = { "Add","Subtract","ReverseSubtract","Min","Max" };
						int e = static_cast<int>(m_BlendEquation);
						ImGui::Combo("Equation", &e, eq, IM_ARRAYSIZE(eq));
						SetBlendEquation((BlendEquation)e);
						ImGui::Unindent();
					}
				}

				// Point Size GLSL
				ImGui::Checkbox("Point Size GLSL", &m_PointSize);
				if (m_PointSize) {
					ImGui::Indent();
					ImGui::SliderFloat("Point Size", &m_PointSizeValue, 1.0f, 100.0f);
					SetPointSizeValue(m_PointSizeValue);
					ImGui::Unindent();
				}
			}
		}

		void ResetFrame() const {
			// Clear Buffers
			glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
			// V‑Sync
			glfwSwapInterval(m_VSync ? 1 : 0);
		}

	private:
		Window& m_Window;
		glm::vec4 m_ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };

		// V-Sync
		bool m_VSync = true;
		
		// Depth Test
		bool m_DepthTest = false;
		DepthFunc m_DepthFunc = DepthFunc::Less;
		bool m_DepthMask = true;
		
		// Cull Face
		bool m_CullFace = false;
		CullFaceMode m_CullFaceMode = CullFaceMode::Back;
		FrontFace m_FrontFace = FrontFace::CCW;
		
		// Stencil Test
		bool m_StencilTest = false;
		StencilFunc m_StencilFunc = StencilFunc::Always;
		int m_StencilRef = 0;
		int m_StencilMask = 0xFF;
		unsigned int m_StencilWriteMask = 0xFF;
		StencilOp m_StencilFail = StencilOp::Keep;
		StencilOp m_StencilDepthFail = StencilOp::Keep;
		StencilOp m_StencilDepthPass = StencilOp::Keep;
		
		// Alpha Blending
		bool m_AlphaBlending = false;
		BlendFactor m_BlendSrcRGB = BlendFactor::SrcAlpha;
		BlendFactor m_BlendDstRGB = BlendFactor::OneMinusSrcAlpha;
		BlendFactor m_BlendSrcAlpha = BlendFactor::One;
		BlendFactor m_BlendDstAlpha = BlendFactor::Zero;
		BlendEquation m_BlendEquation = BlendEquation::Add;

		// Point size
		bool m_PointSize = false;
		float m_PointSizeValue = 1.0f;

		static GLenum _toGLDepthFunc(DepthFunc d) {
			switch (d) {
				case DepthFunc::Never:    return GL_NEVER;
				case DepthFunc::Less:     return GL_LESS;
				case DepthFunc::Equal:    return GL_EQUAL;
				case DepthFunc::Lequal:   return GL_LEQUAL;
				case DepthFunc::Greater:  return GL_GREATER;
				case DepthFunc::NotEqual: return GL_NOTEQUAL;
				case DepthFunc::Gequal:   return GL_GEQUAL;
				case DepthFunc::Always:   return GL_ALWAYS;
			}
			return GL_LESS;
		}

		static GLenum _toGLCullFace(CullFaceMode m) {
			switch (m) {
				case CullFaceMode::Front:        return GL_FRONT;
				case CullFaceMode::Back:         return GL_BACK;
				case CullFaceMode::FrontAndBack: return GL_FRONT_AND_BACK;
			}
			return GL_BACK;
		}

		static GLenum _toGLFrontFace(FrontFace m) {
			switch (m) {
				case FrontFace::CW:  return GL_CW;
				case FrontFace::CCW: return GL_CCW;
			}
			return GL_CCW;
		}

		static GLenum _toGLStencilFunc(StencilFunc f) {
			switch (f) {
				case StencilFunc::Always:    return GL_ALWAYS;
				case StencilFunc::Never:     return GL_NEVER;
				case StencilFunc::Less:      return GL_LESS;
				case StencilFunc::Equal:     return GL_EQUAL;
				case StencilFunc::Lequal:    return GL_LEQUAL;
				case StencilFunc::Greater:   return GL_GREATER;
				case StencilFunc::NotEqual:  return GL_NOTEQUAL;
				case StencilFunc::Gequal:    return GL_GEQUAL;
			}
			return GL_ALWAYS;
		}

		static GLenum _toGLStencilOp(StencilOp op) {
			switch (op) {
				case StencilOp::Keep:        return GL_KEEP;
				case StencilOp::Zero:        return GL_ZERO;
				case StencilOp::Replace:     return GL_REPLACE;
				case StencilOp::Incr:        return GL_INCR;
				case StencilOp::Decr:        return GL_DECR;
				case StencilOp::IncrWrap:    return GL_INCR_WRAP;
				case StencilOp::DecrWrap:    return GL_DECR_WRAP;
				case StencilOp::Invert:      return GL_INVERT;
			}
			return GL_KEEP;
		}

		static GLenum _toGLBlendFactor(BlendFactor f) {
			switch (f) {
				case BlendFactor::Zero:                 return GL_ZERO;
				case BlendFactor::One:                  return GL_ONE;
				case BlendFactor::SrcColor:             return GL_SRC_COLOR;
				case BlendFactor::OneMinusSrcColor:     return GL_ONE_MINUS_SRC_COLOR;
				case BlendFactor::DstColor:             return GL_DST_COLOR;
				case BlendFactor::OneMinusDstColor:     return GL_ONE_MINUS_DST_COLOR;
				case BlendFactor::SrcAlpha:             return GL_SRC_ALPHA;
				case BlendFactor::OneMinusSrcAlpha:     return GL_ONE_MINUS_SRC_ALPHA;
				case BlendFactor::DstAlpha:             return GL_DST_ALPHA;
				case BlendFactor::OneMinusDstAlpha:     return GL_ONE_MINUS_DST_ALPHA;
				case BlendFactor::ConstantColor:        return GL_CONSTANT_COLOR;
				case BlendFactor::OneMinusConstantColor:return GL_ONE_MINUS_CONSTANT_COLOR;
				case BlendFactor::ConstantAlpha:        return GL_CONSTANT_ALPHA;
				case BlendFactor::OneMinusConstantAlpha:return GL_ONE_MINUS_CONSTANT_ALPHA;
				case BlendFactor::SrcAlphaSaturate:     return GL_SRC_ALPHA_SATURATE;
			}
			return GL_ONE;
		}

		static GLenum _toGLBlendEquation(BlendEquation e) {
			switch (e) {
				case BlendEquation::Add:				return GL_FUNC_ADD;
				case BlendEquation::Subtract:			return GL_FUNC_SUBTRACT;
				case BlendEquation::ReverseSubtract:	return GL_FUNC_REVERSE_SUBTRACT;
				case BlendEquation::Min:				return GL_MIN;
				case BlendEquation::Max:				return GL_MAX;
			}
			return GL_FUNC_ADD;
		}
	};
}