#pragma once

#include "Engine/Core/Layer.h"
#include "Engine/Core/Application.h"

#include <thread>
#include <functional>
#include <memory>
#include <atomic>
#include <mutex>

namespace Engine
{
    class TransitionLayer : public Layer, public std::enable_shared_from_this<TransitionLayer>
    {
    public:
        using LoadFunction = std::function<std::shared_ptr<Layer>(std::atomic<bool>& abortFlag)>;

        TransitionLayer(LoadFunction loadFunction);
        virtual ~TransitionLayer() { AbortLoading(); }

        void OnAttach() override;
        inline void OnDetach() override { AbortLoading(); }
        void OnUpdate(float deltaTime) override;

        void AbortLoading();
        inline void SetTransitionReady(bool ready) { m_TransitionReady = ready; }

    protected:
        std::atomic<bool> m_LoadingCompleted = false;
        std::atomic<bool> m_TransitionReady = false;

    private:
        LoadFunction m_LoadFunction;
        std::thread m_Thread;
        std::atomic<bool> m_NextLayerCreated{ false };
        std::shared_ptr<Layer> m_NextLayer;
        std::mutex m_Mutex;
        bool m_HasPushedNextLayer{ false };

        std::atomic<bool> m_AbortRequested{ false };
    };
}