#pragma once

#include "WAVLoader.h"

#include <AL/al.h>

#include <cstdint>
#include <string>

namespace Engine
{
    class AudioBuffer
    {
    public:
        explicit AudioBuffer(const WAVData& wav);
        ~AudioBuffer();

        AudioBuffer(AudioBuffer&& other) noexcept;
        AudioBuffer& operator=(AudioBuffer&& other) noexcept;

        AudioBuffer(const AudioBuffer&) = delete;
        AudioBuffer& operator=(const AudioBuffer&) = delete;

        static AudioBuffer LoadFromFile(const std::string& filepath);

        inline ALuint Id() const noexcept { return m_Buffer; }
        inline uint32_t SampleRate() const noexcept { return m_SampleRate; }
        inline float Duration() const noexcept { return m_Duration; }

    private:
        ALuint m_Buffer = 0;
        uint32_t m_SampleRate = 0;
        float m_Duration = 0.0f;

        void ValidateWAVData(const WAVData& wav) const;
        static ALenum GetFormatFromWAV(const WAVData& wav);
        void CalculateDuration(const WAVData& wav) noexcept;
    };
}