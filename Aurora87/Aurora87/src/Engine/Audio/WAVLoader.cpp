#include "WAVLoader.h"

namespace Engine
{
	WAVData WAVLoader::Load(const std::string& filepath)
	{
		drwav wav;

		if (!drwav_init_file(&wav, filepath.c_str(), nullptr))
		{
			throw std::runtime_error("WAVLoader::Load: Error when opening: " + filepath);
		}

		// Validación de canales
		if (wav.channels < 1 || wav.channels > 2)
		{
			drwav_uninit(&wav);
			throw std::runtime_error("WAVLoader::Load: Channels not supported (" +
				std::to_string(wav.channels) + ") en: " + filepath);
		}

		// Validación de bits por muestra
		if (wav.bitsPerSample != 16)
		{
			drwav_uninit(&wav);
			throw std::runtime_error("WAVLoader::Load: Bits per sample are not 16 (" +
				std::to_string(wav.bitsPerSample) + ") in: " + filepath);
		}

		const ALenum format = (wav.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
		const drwav_uint64 totalFrames = wav.totalPCMFrameCount;
		const drwav_uint64 totalSamples = totalFrames * wav.channels;

		std::vector<int16_t> pcmData(static_cast<size_t>(totalSamples));

		// Leer datos
		const drwav_uint64 framesRead = drwav_read_pcm_frames_s16(&wav, totalFrames, pcmData.data());
		drwav_uninit(&wav);

		if (framesRead != totalFrames)
		{
			throw std::runtime_error("WAVLoader::Load: Error reading frames in: " + filepath +
				"\nFrames read: " + std::to_string(framesRead) + "/" + std::to_string(totalFrames));
		}

		return WAVData
		{
			std::move(pcmData),
			static_cast<uint32_t>(wav.channels),
			static_cast<uint32_t>(wav.sampleRate),
			16,  // We have already validated that they are 16 bits
			format,
			static_cast<uint64_t>(totalFrames)
		};
	}
}