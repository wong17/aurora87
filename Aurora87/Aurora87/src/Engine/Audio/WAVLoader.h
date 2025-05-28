#pragma once

#include <AL/al.h>
#include <dr_libs/dr_wav.h>

#include <stdexcept>
#include <vector>
#include <string>
#include <optional>
#include <cstdint>

namespace Engine
{
	struct WAVData 
	{
		std::vector<int16_t> Data;		// Audio samples en formato signed 16-bit PCM
		uint32_t Channels;				// Número de canales (1 = mono, 2 = stereo)
		uint32_t SampleRate;			// Frecuencia de muestreo en Hz
		uint32_t BitsPerSample;			// Bits por muestra (siempre 16)
		ALenum Format;					// Formato OpenAL (AL_FORMAT_MONO16/AL_FORMAT_STEREO16)
		uint64_t TotalPCMFrameCount;	// Número total de frames PCM en el archivo WAV
	};

	class WAVLoader
	{
	public:
		static WAVData Load(const std::string& filepath);
	};
}