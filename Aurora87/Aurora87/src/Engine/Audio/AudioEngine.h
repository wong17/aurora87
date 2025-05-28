#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <functional>
#include <cstring>

namespace Engine
{
	enum class InitResult 
	{
		OK,
		DeviceFail,
		// Error al abrir dispositivo
		ContextFail,		// Error al crear contexto
		MakeCurrentFail		// Error al activar contexto
	};

	class AudioEngine 
	{
	public:
		using ErrorCallback = std::function<void(const std::string&)>;

		AudioEngine() = default;
		~AudioEngine();

		AudioEngine(AudioEngine&& other) noexcept;
		AudioEngine& operator=(AudioEngine&& other) noexcept;

		AudioEngine(const AudioEngine&) = delete;
		AudioEngine& operator=(const AudioEngine&) = delete;

		InitResult Initialize(const std::optional<std::string>& deviceName = std::nullopt);
		void Shutdown();

		// Configuración del oyente
		void SetListenerPosition(float x, float y, float z);
		void SetListenerVelocity(float x, float y, float z);
		void SetListenerOrientation(const float* forwardAndUp);
		void SetListenerGain(float gain);

		// Dispositivos y contexto
		static std::vector<std::string> GetDevices();
		bool HasExtension(const std::string& ext) const;
		void PrintDeviceInfo() const;
		bool SuspendContext();
		bool RestoreContext();

		void SetErrorCallback(ErrorCallback cb) { m_ErrorCallback = cb; }

		bool IsValid() const { return m_Device && m_Context; }
		ALCdevice* GetNativeDevice() const { return m_Device; }
		ALCcontext* GetNativeContext() const { return m_Context; }

	private:
		ALCdevice* m_Device = nullptr;
		ALCcontext* m_Context = nullptr;
		ErrorCallback m_ErrorCallback;

		bool OpenDevice(const char* name);
		void CloseDevice();
		bool CheckALCErrors() const;
		bool CheckALErrors();
		bool MakeContextCurrent(bool active);
	};
}