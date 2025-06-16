#include "AudioBuffer.h"

namespace Engine
{
    AudioBuffer::AudioBuffer(const WAVData& wav)
    {
        ValidateWAVData(wav);

        alGenBuffers(1, &m_Buffer);
        if (ALenum err = alGetError(); err != AL_NO_ERROR) 
        {
            throw std::runtime_error("AudioBuffer::AudioBuffer: Error generating OpenAL buffer: " + 
                std::string(alGetString(err)));
        }

        const ALenum format = (wav.Channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;

        alBufferData(m_Buffer, format, wav.Data.data(),
            static_cast<ALsizei>(wav.Data.size() * sizeof(int16_t)),
            static_cast<ALsizei>(wav.SampleRate));

        if (ALenum err = alGetError(); err != AL_NO_ERROR) 
        {
            alDeleteBuffers(1, &m_Buffer);
            throw std::runtime_error("AudioBuffer::AudioBuffer: Error loading audio: " + std::string(alGetString(err)));
        }

        m_SampleRate = wav.SampleRate;
        CalculateDuration(wav);
    }

    AudioBuffer::~AudioBuffer()
    {
        if (m_Buffer) 
        {
            alDeleteBuffers(1, &m_Buffer);
        }
    }

    AudioBuffer::AudioBuffer(AudioBuffer&& other) noexcept
        : m_Buffer(other.m_Buffer),
        m_SampleRate(other.m_SampleRate),
        m_Duration(other.m_Duration)
    {
        other.m_Buffer = 0;
    }

    AudioBuffer& AudioBuffer::operator=(AudioBuffer&& other) noexcept
    {
        if (this != &other) 
        {
            if (m_Buffer) alDeleteBuffers(1, &m_Buffer);

            m_Buffer = other.m_Buffer;
            m_SampleRate = other.m_SampleRate;
            m_Duration = other.m_Duration;

            other.m_Buffer = 0;
        }

        return *this;
    }

    AudioBuffer AudioBuffer::LoadFromFile(const std::string& filepath)
    {
        WAVData wav = WAVLoader::Load(filepath);
        return AudioBuffer(wav);
    }

    void AudioBuffer::ValidateWAVData(const WAVData& wav) const
    {
        if (wav.Data.empty()) 
        {
            throw std::invalid_argument("AudioBuffer::ValidateWAVData: WAV data is empty");
        }

        if (wav.Channels != 1 && wav.Channels != 2) 
        {
            throw std::invalid_argument("AudioBuffer::ValidateWAVData: Unsupported number of channels (" +
                std::to_string(wav.Channels) + ")");
        }

        if (wav.SampleRate == 0) 
        {
            throw std::invalid_argument("AudioBuffer::ValidateWAVData: Invalid sample rate (0 Hz)");
        }

        if (wav.BitsPerSample != 16) 
        {
            throw std::invalid_argument("AudioBuffer::ValidateWAVData: Only 16-bit samples are supported");
        }
    }

    ALenum AudioBuffer::GetFormatFromWAV(const WAVData& wav)
    {
        if (wav.BitsPerSample == 16) 
        {
            return wav.Channels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
        }
        
        throw std::invalid_argument("Unsupported bit depth: " + std::to_string(wav.BitsPerSample));
    }

    void AudioBuffer::CalculateDuration(const WAVData& wav) noexcept
    {
        const uint64_t totalFrames = wav.TotalPCMFrameCount;
        m_Duration = static_cast<float>(totalFrames) / static_cast<float>(wav.SampleRate);
    }
}