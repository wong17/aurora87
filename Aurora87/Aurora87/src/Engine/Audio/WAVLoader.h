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
		std::vector<int16_t> Data;		// Audio samples in 16-bit PCM signed format
		uint32_t Channels;				// Number of channels (1 = mono, 2 = stereo)
		uint32_t SampleRate;			// Sampling frequency in Hz
		uint32_t BitsPerSample;			// Bits per sample (always 16)
		ALenum Format;					// OpenAL format (AL_FORMAT_MONO16/AL_FORMAT_STEREO16)
		uint64_t TotalPCMFrameCount;	// Total number of PCM frames in the WAV file
	};

	class WAVLoader
	{
	public:
		static WAVData Load(const std::string& filepath);
	};
}