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

        if (m_NextLayerCreated && !m_LoadingCompleted)
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            if (m_NextLayer)
            {
                m_LoadingCompleted = true;
            }
        }

        if (m_LoadingCompleted && m_TransitionReady && !m_HasPushedNextLayer)
        {
            {
                std::lock_guard<std::mutex> lock(m_Mutex);
                if (m_NextLayer)
                {
                    Engine::Application::Get().PushLayer(m_NextLayer);
                }
            }

            m_HasPushedNextLayer = true;
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