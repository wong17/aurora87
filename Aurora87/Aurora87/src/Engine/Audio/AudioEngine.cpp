#include "AudioEngine.h"

namespace Engine
{
	AudioEngine::AudioEngine(AudioEngine&& other) noexcept
		: m_Device(other.m_Device),
		m_Context(other.m_Context),
		m_ErrorCallback(std::move(other.m_ErrorCallback))
	{
		other.m_Device = nullptr;
		other.m_Context = nullptr;
	}

	AudioEngine& AudioEngine::operator=(AudioEngine&& other) noexcept
	{
		if (this != &other)
		{
			Shutdown();
			m_Device = other.m_Device;
			m_Context = other.m_Context;
			m_ErrorCallback = std::move(other.m_ErrorCallback);
			other.m_Device = nullptr;
			other.m_Context = nullptr;
		}

		return *this;
	}

	AudioEngine::~AudioEngine()
	{
		Shutdown();
	}

	InitResult AudioEngine::Initialize(const std::optional<std::string>& deviceName)
	{
		if (!OpenDevice(deviceName ? deviceName->c_str() : nullptr))
			return InitResult::DeviceFail;

		m_Context = alcCreateContext(m_Device, nullptr);
		if (!m_Context || !CheckALCErrors())
			return InitResult::ContextFail;

		if (!MakeContextCurrent(true))
			return InitResult::MakeCurrentFail;

		return InitResult::OK;
	}

	void AudioEngine::Shutdown()
	{
		MakeContextCurrent(false);

		if (m_Context)
		{
			alcDestroyContext(m_Context);
			m_Context = nullptr;
		}

		CloseDevice();
	}

	void AudioEngine::SetListenerPosition(float x, float y, float z)
	{
		alListener3f(AL_POSITION, x, y, z);
		CheckALErrors();
	}

	void AudioEngine::SetListenerVelocity(float x, float y, float z)
	{
		alListener3f(AL_VELOCITY, x, y, z);
		CheckALErrors();
	}

	void AudioEngine::SetListenerOrientation(const float* forwardAndUp)
	{
		alListenerfv(AL_ORIENTATION, forwardAndUp);
		CheckALErrors();
	}

	void AudioEngine::SetListenerGain(float gain)
	{
		alListenerf(AL_GAIN, gain);
		CheckALErrors();
	}

	bool AudioEngine::OpenDevice(const char* name)
	{
		m_Device = alcOpenDevice(name);
		if (!m_Device)
		{
			if (m_ErrorCallback)
			{
				std::string msg = "AudioEngine::OpenDevice: Error opening the device";
				msg += name ? name : "default";
				m_ErrorCallback(msg);
			}
			return false;
		}
		return true;
	}

	void AudioEngine::CloseDevice()
	{
		if (m_Device)
		{
			alcCloseDevice(m_Device);
			m_Device = nullptr;
		}
	}

	std::vector<std::string> AudioEngine::GetDevices()
	{
		const ALCchar* devices = alcGetString(nullptr, ALC_DEVICE_SPECIFIER);
		std::vector<std::string> result;

		while (devices && *devices != '\0')
		{
			result.emplace_back(devices);
			devices += strlen(devices) + 1;
		}
		return result;
	}

	bool AudioEngine::HasExtension(const std::string& ext) const
	{
		return m_Device && alcIsExtensionPresent(m_Device, ext.c_str()) == AL_TRUE;
	}

	bool AudioEngine::MakeContextCurrent(bool active)
	{
		const ALCboolean result = alcMakeContextCurrent(active ? m_Context : nullptr);
		return CheckALCErrors() && (result == ALC_TRUE);
	}

	bool AudioEngine::SuspendContext()
	{
		alcSuspendContext(m_Context);
		return CheckALCErrors();
	}

	bool AudioEngine::RestoreContext()
	{
		return MakeContextCurrent(true);
	}

	bool AudioEngine::CheckALCErrors() const
	{
		const ALCenum error = alcGetError(m_Device);
		if (error == ALC_NO_ERROR) return true;

		if (m_ErrorCallback)
		{
			m_ErrorCallback("ALC Error: " + std::string(alcGetString(m_Device, error)));
		}
		return false;
	}

	bool AudioEngine::CheckALErrors()
	{
		const ALenum error = alGetError();
		if (error == AL_NO_ERROR) return true;

		if (m_ErrorCallback)
		{
			m_ErrorCallback("AL Error: " + std::string(alGetString(error)));
		}
		return false;
	}

	void AudioEngine::PrintDeviceInfo() const
	{
		if (!IsValid()) return;

		std::cout << "Device: " << alcGetString(m_Device, ALC_DEVICE_SPECIFIER) << "\n"
			<< "OpenAL Version: " << alGetString(AL_VERSION) << "\n"
			<< "Renderer: " << alGetString(AL_RENDERER) << "\n"
			<< "Vendor: " << alGetString(AL_VENDOR) << "\n";
	}
}