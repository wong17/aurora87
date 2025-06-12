#pragma once

#include "Engine/Events/Event.h"

#include <iostream>
#include <string>

namespace Engine
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		virtual bool IsLoaded() const { return m_IsLoaded; }
		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		bool m_IsLoaded = false;
		std::string m_DebugName;
	};
}