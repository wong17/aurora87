#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/efx.h>

#include <vector>
#include <functional>
#include <stdexcept>
#include <iostream>
#include <algorithm>

namespace Engine
{
	class AudioSource
	{
	public:
        enum class State 
        { 
            Initial, 
            Playing, 
            Paused, 
            Stopped 
        };

        using StateCallback = std::function<void(State)>;

        AudioSource();
        ~AudioSource();

        AudioSource(const AudioSource&) = delete;
        AudioSource& operator=(const AudioSource&) = delete;

        AudioSource(AudioSource&& other) noexcept;
        AudioSource& operator=(AudioSource&& other) noexcept;

        void AttachBuffer(ALuint buffer);
        void Play();
        void Pause();
        void Stop();
        State GetState() const;

        void Update(float deltaTime);

        void SetGain(float gain);
        void SetPitch(float pitch);
        void SetLooping(bool loop);

		float GetGain() const;
		float GetPitch() const;

        void SetPosition(float x, float y, float z);
        void SetVelocity(float x, float y, float z);
        void SetDirection(float x, float y, float z);
        void SetSpatialProperties(float refDist, float maxDist, float rolloff);

        void SetCone(float innerAngle, float outerAngle, float outerGain);

        void QueueBuffers(const std::vector<ALuint>& buffers);
        ALuint UnqueueProcessedBuffers();

        void AttachFilter(ALuint filter);
        void AttachEffectSlot(ALuint slot, ALint sendIndex = 0);

        void SetStateCallback(StateCallback cb);

        void FadeTo(float targetGain, float duration);
        void FadeIn(float duration) { FadeTo(1.0f, duration); }
        void FadeOut(float duration) { FadeTo(0.0f, duration); }

    private:
        ALuint m_Source = 0;
        State  m_PrevState = State::Initial;
        StateCallback m_StateCallback = nullptr;

        // Fade parametros
        bool m_Fading = false;
        float m_FadeCurrentTime = 0.0f;
        float m_FadeDuration = 0.0f;
        float m_FadeStartGain = 0.0f;
        float m_FadeTargetGain = 0.0f;

        void CheckALError(const char* where) const;
        void NotifyStateChange(State newState);
	};
}