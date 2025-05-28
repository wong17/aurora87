#include "AudioSource.h"

namespace Engine
{
    AudioSource::AudioSource() 
    {
        alGenSources(1, &m_Source);
        if (ALenum err = alGetError(); err != AL_NO_ERROR) 
        {
            throw std::runtime_error("Failed to create audio source: " +
                std::string(alGetString(err)));
        }
    }

    AudioSource::~AudioSource() 
    {
        if (m_Source) 
            alDeleteSources(1, &m_Source);
    }

    AudioSource::AudioSource(AudioSource&& other) noexcept
        : m_Source(other.m_Source),
        m_PrevState(other.m_PrevState),
        m_StateCallback(std::move(other.m_StateCallback)),
        m_Fading(other.m_Fading),
        m_FadeCurrentTime(other.m_FadeCurrentTime),
        m_FadeDuration(other.m_FadeDuration),
        m_FadeStartGain(other.m_FadeStartGain),
        m_FadeTargetGain(other.m_FadeTargetGain)
    {
        other.m_Source = 0;
    }

    AudioSource& AudioSource::operator=(AudioSource&& other) noexcept
    {
        if (this != &other) 
        {
            if (m_Source) alDeleteSources(1, &m_Source);

            m_Source = other.m_Source;
            other.m_Source = 0;

            m_PrevState = other.m_PrevState;
            m_StateCallback = std::move(other.m_StateCallback);
            m_Fading = other.m_Fading;
            m_FadeCurrentTime = other.m_FadeCurrentTime;
            m_FadeDuration = other.m_FadeDuration;
            m_FadeStartGain = other.m_FadeStartGain;
            m_FadeTargetGain = other.m_FadeTargetGain;
        }

        return *this;
    }

    void AudioSource::AttachBuffer(ALuint buffer) 
    {
        alSourcei(m_Source, AL_BUFFER, static_cast<ALint>(buffer));
        CheckALError("AttachBuffer");
    }

    void AudioSource::Play() 
    {
        alSourcePlay(m_Source);
        CheckALError("Play");
    }

    void AudioSource::Pause() 
    {
        alSourcePause(m_Source);
        CheckALError("Pause");
    }

    void AudioSource::Stop() 
    {
        alSourceStop(m_Source);
        CheckALError("Stop");
    }

    AudioSource::State AudioSource::GetState() const 
    {
        ALint state;
        alGetSourcei(m_Source, AL_SOURCE_STATE, &state);

        switch (state) 
        {
            case AL_PLAYING: return State::Playing;
            case AL_PAUSED: return State::Paused;
            case AL_STOPPED: return State::Stopped;
            default: return State::Initial;
        }
    }

    void AudioSource::Update(float deltaTime)
    {
        const State current = GetState();
        if (current != m_PrevState) 
        {
            NotifyStateChange(current);
            m_PrevState = current;
        }

        if (m_Fading) 
        {
            m_FadeCurrentTime += deltaTime;

            if (m_FadeCurrentTime >= m_FadeDuration) 
            {
                SetGain(m_FadeTargetGain);
                m_Fading = false;
            }
            else 
            {
                const float t = m_FadeCurrentTime / m_FadeDuration;
                const float gain = m_FadeStartGain + t * (m_FadeTargetGain - m_FadeStartGain);
                SetGain(gain);
            }
        }
    }

    void AudioSource::SetGain(float gain) 
    {
        if (gain < 0.0f) gain = 0.0f;

        alSourcef(m_Source, AL_GAIN, gain);
        CheckALError("SetGain");
    }

    void AudioSource::SetPitch(float pitch) 
    {
        alSourcef(m_Source, AL_PITCH, pitch);
        CheckALError("SetPitch");
    }

    void AudioSource::SetLooping(bool loop) 
    {
        alSourcei(m_Source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
        CheckALError("SetLooping");
    }

    float AudioSource::GetGain() const
    {
        float gain;
        alGetSourcef(m_Source, AL_GAIN, &gain);
        return gain;
    }

    float AudioSource::GetPitch() const
    {
        float pitch;
        alGetSourcef(m_Source, AL_PITCH, &pitch);
		return pitch;
    }

    void AudioSource::SetPosition(float x, float y, float z) 
    {
        alSource3f(m_Source, AL_POSITION, x, y, z);
        CheckALError("SetPosition");
    }

    void AudioSource::SetVelocity(float x, float y, float z) 
    {
        alSource3f(m_Source, AL_VELOCITY, x, y, z);
        CheckALError("SetVelocity");
    }

    void AudioSource::SetDirection(float x, float y, float z) 
    {
        alSource3f(m_Source, AL_DIRECTION, x, y, z);
        CheckALError("SetDirection");
    }

    void AudioSource::SetSpatialProperties(float refDist, float maxDist, float rolloff) {
        // Validación de parámetros
        refDist = std::max(0.01f, refDist);
        maxDist = std::max(refDist + 0.01f, maxDist);
        rolloff = std::clamp(rolloff, 0.0f, 10.0f);

        alSourcef(m_Source, AL_REFERENCE_DISTANCE, refDist);
        alSourcef(m_Source, AL_MAX_DISTANCE, maxDist);
        alSourcef(m_Source, AL_ROLLOFF_FACTOR, rolloff);
        CheckALError("SetSpatialProperties");
    }

    void AudioSource::SetCone(float innerAngle, float outerAngle, float outerGain) 
    {
        alSourcef(m_Source, AL_CONE_INNER_ANGLE, innerAngle);
        alSourcef(m_Source, AL_CONE_OUTER_ANGLE, outerAngle);
        alSourcef(m_Source, AL_CONE_OUTER_GAIN, outerGain);
        CheckALError("SetCone");
    }

    void AudioSource::QueueBuffers(const std::vector<ALuint>& buffers) 
    {
        alSourceQueueBuffers(m_Source, static_cast<ALsizei>(buffers.size()), buffers.data());
        CheckALError("QueueBuffers");
    }

    ALuint AudioSource::UnqueueProcessedBuffers() 
    {
        ALint processed = 0;
        alGetSourcei(m_Source, AL_BUFFERS_PROCESSED, &processed);

        if (processed > 0) 
        {
            ALuint buffer;
            alSourceUnqueueBuffers(m_Source, 1, &buffer);
            CheckALError("UnqueueBuffers");
            return buffer;
        }

        return 0;
    }

    void AudioSource::AttachFilter(ALuint filter) 
    {
        alSourcei(m_Source, AL_DIRECT_FILTER, filter);
        CheckALError("AttachFilter");
    }

    void AudioSource::AttachEffectSlot(ALuint slot, ALint sendIndex)
    {
        alSource3i(m_Source,
            AL_AUXILIARY_SEND_FILTER,
            static_cast<ALint>(slot),
            sendIndex,
            AL_FILTER_NULL);
        CheckALError("AttachEffectSlot");
    }

    void AudioSource::SetStateCallback(StateCallback cb) 
    {
        m_StateCallback = std::move(cb);
    }

    void AudioSource::FadeTo(float targetGain, float duration)
    {
        if (duration <= 0.0f) 
        {
            SetGain(targetGain);
            return;
        }

        m_FadeStartGain = GetGain();
        m_FadeTargetGain = targetGain;
        m_FadeDuration = duration;
        m_FadeCurrentTime = 0.0f;
        m_Fading = true;
    }


    void AudioSource::CheckALError(const char* where) const 
    {
        ALenum err = alGetError();
        if (err != AL_NO_ERROR) 
        {
            std::cerr << "OpenAL error in " << where << ": " << alGetString(err) << "\n";
        }
    }

    void AudioSource::NotifyStateChange(State newState) 
    {
        if (m_StateCallback) m_StateCallback(newState);
    }
}