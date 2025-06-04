#pragma once

#include "UniformBufferElement.h"

#include <initializer_list>
#include <cstdint>
#include <vector>
#include <stdexcept>

namespace Engine
{
	class UniformBufferLayout
	{
    public:
        UniformBufferLayout() = default;
        UniformBufferLayout(std::initializer_list<UniformBufferElement> elements)
            : m_Elements(elements)
        {
            CalculateOffsetsAndStride();
        }
        UniformBufferLayout(const std::vector<UniformBufferElement>& elements)
            : m_Elements(elements)
        {
            CalculateOffsetsAndStride();
        }
		~UniformBufferLayout() = default;

        inline const std::vector<UniformBufferElement>& GetElements() const { return m_Elements; }
        inline void SetElements(const std::vector<UniformBufferElement>& elements) 
        { 
            m_Elements = elements; 
            CalculateOffsetsAndStride(); 
		}
        inline uint32_t GetStride() const { return m_Stride; }

        const UniformBufferElement& GetElement(const std::string& name) const 
        {
            for (auto& e : m_Elements)
                if (e.Name == name) 
                    return e;
            
            throw std::runtime_error("UniformBufferLayout::GetElement: Elemento '" + name + "' no encontrado");
        }

    private:
        void CalculateOffsetsAndStride() 
        {
            uint32_t offset = 0;
            m_Stride = 0;
            for (auto& element : m_Elements) 
            {
                uint32_t align = Std140BaseAlignment(element.Type);
                uint32_t size = Std140Size(element.Type);

                uint32_t padding = (align - (offset % align)) % align;
                offset += padding;

                element.Offset = offset;

                offset += size;
            }
            m_Stride = offset;

            if (m_Stride % 16 != 0)
                m_Stride += 16 - (m_Stride % 16);
        }
    private:
        std::vector<UniformBufferElement> m_Elements;
        uint32_t m_Stride = 0;
	};
}