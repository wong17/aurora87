#pragma once

namespace Engine
{
	class ILightTween 
	{
	public:
		virtual ~ILightTween() = default;
		
		virtual void Update(float dt) = 0;
		virtual bool IsActive() const = 0;
	};
}