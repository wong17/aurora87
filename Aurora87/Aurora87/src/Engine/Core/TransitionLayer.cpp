#include "TransitionLayer.h"

namespace Engine
{
    TransitionLayer::TransitionLayer(LoadFunction loadFunction)
        : m_LoadFunction(std::move(loadFunction)) { }

    void TransitionLayer::OnAttach()
    {
        auto self = shared_from_this();

        m_Thread = std::thread([self]() 
            {
                auto nextLayer = self->m_LoadFunction(self->m_AbortRequested);
                
                if (!self->m_AbortRequested.load())
                {
                    std::lock_guard<std::mutex> lock(self->m_Mutex);
                    self->m_NextLayer = std::move(nextLayer);
                    self->m_NextLayerCreated = true;
                }
            });
    }

    void TransitionLayer::OnUpdate(float deltaTime)
    {
        if (m_AbortRequested)
            return;

        if (m_NextLayerCreated && !m_HasPushedNextLayer)
        {
            std::shared_ptr<Layer> nextLayerCopy;
            {
                std::lock_guard<std::mutex> lock(m_Mutex);
                nextLayerCopy = m_NextLayer;
            }

            if (nextLayerCopy)
            {
                Engine::Application::Get().PushLayer(nextLayerCopy);
                m_HasPushedNextLayer = true;
            }
        }

        if (m_HasPushedNextLayer && m_NextLayer && m_NextLayer->IsLoaded())
        {
            Engine::Application::Get().PopLayer(shared_from_this());
        }
    }

    void TransitionLayer::AbortLoading()
    {
        m_AbortRequested = true;

        if (m_Thread.joinable())
        {
            m_Thread.join();
        }

        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_NextLayer.reset();
            m_NextLayerCreated = false;
            m_HasPushedNextLayer = false;
        }
    }
}