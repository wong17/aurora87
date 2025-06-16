#pragma once

#include "AudioBuffer.h"
#include "AudioSource.h"

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <mutex>

namespace Engine
{
    class AudioManager 
    {
    public:
        AudioManager() = default;
        ~AudioManager() = default;

        // Buffer
        std::shared_ptr<AudioBuffer> LoadBuffer(const std::string& name, const std::string& filepath);
        std::shared_ptr<AudioBuffer> GetBuffer(const std::string& name) const;
        bool BufferExists(const std::string& name) const;
        bool RemoveBuffer(const std::string& name);
        void ClearBuffers();
        std::vector<std::string> ListBuffers() const;

        // Source (pool)
        std::shared_ptr<AudioSource> AcquireSource(const std::string& bufferName);
        void ReleaseSource(const std::shared_ptr<AudioSource>& source);
        std::vector<std::shared_ptr<AudioSource>> AcquireSources(const std::vector<std::string>& bufferNames);

    private:
        mutable std::mutex m_Mutex;
        std::unordered_map<std::string, std::shared_ptr<AudioBuffer>> m_BufferCache;
        // AudioSource pool for reuse
        std::vector<std::shared_ptr<AudioSource>> m_FreeSources;
    };
}