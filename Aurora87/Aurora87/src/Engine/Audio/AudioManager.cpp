#include "AudioManager.h"

namespace Engine
{
    static std::shared_ptr<AudioSource> MakeSourceWithBuffer(const std::shared_ptr<AudioBuffer>& buf) 
    {
        if (!buf)
			throw std::runtime_error("AudioManager::MakeSourceWithBuffer: No se puede crear una fuente de audio con un buffer nulo.");

        auto src = std::make_shared<AudioSource>();
        src->AttachBuffer(buf->Id());
        return src;
    }

    std::shared_ptr<AudioBuffer> AudioManager::LoadBuffer(const std::string& name, const std::string& filepath) 
    {
        if (name.empty() || filepath.empty()) 
			throw std::invalid_argument("AudioManager::LoadBuffer: Nombre y ruta no pueden estar vacios.");

        std::lock_guard<std::mutex> lock(m_Mutex);
        auto it = m_BufferCache.find(name);
        if (it != m_BufferCache.end())
            return it->second;

        WAVData wav = WAVLoader::Load(filepath);
        auto buffer = std::make_shared<AudioBuffer>(wav);
        m_BufferCache[name] = buffer;
        return buffer;
    }

    std::shared_ptr<AudioBuffer> AudioManager::GetBuffer(const std::string& name) const 
    {
		if (name.empty())
			throw std::invalid_argument("AudioManager::GetBuffer: Nombre no puede estar vacio.");

        std::lock_guard<std::mutex> lock(m_Mutex);
        auto it = m_BufferCache.find(name);

        if (it == m_BufferCache.end())
        {
            std::cerr << "AudioManager::GetBuffer: Buffer '" << name << "' no encontrado." << std::endl;
			return nullptr;
        }
        
        return it->second;
    }

    bool AudioManager::BufferExists(const std::string& name) const 
    {
        if (name.empty())
        {
			std::cerr << "AudioManager::BufferExists: Nombre no puede estar vacio." << std::endl;
            return false;
        }

        std::lock_guard<std::mutex> lock(m_Mutex);
        return m_BufferCache.count(name) > 0;
    }

    bool AudioManager::RemoveBuffer(const std::string& name) 
    {
        if (name.empty())
        {
            std::cerr << "AudioManager::RemoveBuffer: Nombre no puede estar vacio." << std::endl;
            return false;
        }

        std::lock_guard<std::mutex> lock(m_Mutex);
        return m_BufferCache.erase(name) > 0;
    }

    void AudioManager::ClearBuffers() 
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_BufferCache.clear();
    }

    std::vector<std::string> AudioManager::ListBuffers() const 
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        std::vector<std::string> names;
        names.reserve(m_BufferCache.size());
        for (auto& kv : m_BufferCache) names.push_back(kv.first);
        return names;
    }

    std::shared_ptr<AudioSource> AudioManager::AcquireSource(const std::string& bufferName) 
    {
        if (bufferName.empty()) 
        {
            throw std::invalid_argument("AudioManager::AcquireSource: bufferName no puede estar vacío");
        }

        std::lock_guard<std::mutex> lock(m_Mutex);
        // Primero busca el buffer en el cache directamente
        auto itBuf = m_BufferCache.find(bufferName);
        if (itBuf == m_BufferCache.end())
            throw std::runtime_error("AudioManager::AcquireSource: buffer '" + bufferName + "' no existe");

        auto& buffer = itBuf->second;

        // Si hay fuentes libres, reutiliza una
        if (!m_FreeSources.empty())
        {
            auto& src = m_FreeSources.back();
            m_FreeSources.pop_back();
            // Reattach al buffer existente
            src->AttachBuffer(buffer->Id());
            return src;
        }

        // Si no, crea una nueva fuente
        return MakeSourceWithBuffer(buffer);
    }

    void AudioManager::ReleaseSource(const std::shared_ptr<AudioSource>& source) 
    {
        if (!source) 
        {
            throw std::invalid_argument("AudioManager::ReleaseSource: source no puede ser nullptr");
        }

        std::lock_guard<std::mutex> lock(m_Mutex);
        // Resetea estado y pone en pool
        source->Stop();
        m_FreeSources.push_back(source);
    }

    std::vector<std::shared_ptr<AudioSource>> AudioManager::AcquireSources(const std::vector<std::string>& bufferNames) 
    {
        if (bufferNames.empty()) 
        {
            throw std::invalid_argument("AudioManager::AcquireSources: lista de nombres vacía");
        }

        std::vector<std::shared_ptr<AudioSource>> list;
        list.reserve(bufferNames.size());
        for (auto const& name : bufferNames) 
        {
            if (name.empty()) 
            {
                throw std::invalid_argument("AudioManager::AcquireSources: name en la lista no puede estar vacío");
            }

            list.push_back(AcquireSource(name));
        }
        return list;
    }
}