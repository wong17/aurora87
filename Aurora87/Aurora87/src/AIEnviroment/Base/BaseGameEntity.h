#pragma once
#include <string>

namespace AIEnviroment
{
	class BaseGameEntity
	{
	public:
		BaseGameEntity() : m_ID(-1) {}
		virtual ~BaseGameEntity() = default;
		virtual std::string GetType() const = 0;
		virtual void Update() = 0;
		int GetID() const { return m_ID; }
		void SetID(int id) { m_ID = id; }
	private:
		int m_ID;
	};
}
