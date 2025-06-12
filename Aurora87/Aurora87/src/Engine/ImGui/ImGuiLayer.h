#pragma once

#include "Engine/Core/Layer.h"
#include "Engine/Events/Event.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

namespace Engine
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		
		void OnAttach() override;
		void OnDetach() override;
		void OnImGuiRender() override;
		
		void Begin();
		void End();
	};
}