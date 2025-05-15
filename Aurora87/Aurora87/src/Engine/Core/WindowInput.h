#pragma once

#include "Input.h"
#include "Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <utility>
#include <iostream>

namespace Engine
{
	class WindowInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}