/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/UnicornEngine.hpp>
#include <unicorn/utility/Logger.hpp>
#include <unicorn/video/Graphics.hpp>
#include <unicorn/utility/asset/SimpleStorage.hpp>

#include <unicorn/system/Manager.hpp>

#include <unicorn/system/profiler/GamepadProfiler.hpp>
#include <unicorn/system/profiler/KeyProfiler.hpp>
#include <unicorn/system/profiler/MonitorProfiler.hpp>
#include <unicorn/system/profiler/MouseProfiler.hpp>
#include <unicorn/system/profiler/WindowProfiler.hpp>

namespace unicorn
{
UnicornEngine::UnicornEngine()
    : m_isInitialized(false)
    , m_pSystemManager(nullptr)
    , m_pWindowProfiler(nullptr)
    , m_pMonitorProfiler(nullptr)
    , m_pGraphics(nullptr)
{
}

UnicornEngine::~UnicornEngine()
{
    Deinit();
}

bool UnicornEngine::Init()
{
    if (m_isInitialized)
    {
        return false;
    }

    unicorn::utility::asset::SimpleStorage::Instance();
    LOG_INFO("Engine initialization started.");

    m_pSystemManager = new system::Manager();

    m_pWindowProfiler = new system::WindowProfiler(*m_pSystemManager);
    m_pMonitorProfiler = new system::MonitorProfiler(*m_pSystemManager);
    m_pMouseProfiler = new system::MouseProfiler(*m_pSystemManager);
    m_pKeyProfiler = new system::KeyProfiler(*m_pSystemManager);
    m_pGamepadProfiler = new system::GamepadProfiler(*m_pSystemManager);

    m_pSystemManager->Init();

    m_pGraphics = new video::Graphics(*m_pSystemManager);

    if (!m_pGraphics->Init())
    {
        Deinit();

        return false;
    }

    m_isInitialized = true;

    LOG_INFO("Engine initialization finished.");

    return true;
}

void UnicornEngine::Deinit()
{
    if (m_pGraphics)
    {
        m_pGraphics->Deinit();

        delete m_pGraphics;

        m_pGraphics = nullptr;
    }

    if (m_pSystemManager)
    {
        m_pSystemManager->Deinit();

        if (m_pGamepadProfiler)
        {
            delete m_pGamepadProfiler;

            m_pGamepadProfiler = nullptr;
        }

        if (m_pKeyProfiler)
        {
            delete m_pKeyProfiler;

            m_pKeyProfiler = nullptr;
        }

        if (m_pMouseProfiler)
        {
            delete m_pMouseProfiler;

            m_pMouseProfiler = nullptr;
        }

        if (m_pMonitorProfiler)
        {
            delete m_pMonitorProfiler;

            m_pMonitorProfiler = nullptr;
        }

        if (m_pWindowProfiler)
        {
            delete m_pWindowProfiler;

            m_pWindowProfiler = nullptr;
        }

        delete m_pSystemManager;

        m_pSystemManager = nullptr;
    }

    if (m_isInitialized)
    {
        LOG_INFO("Engine shutdown correctly.");
    }

    m_isInitialized = false;
}

void UnicornEngine::Run()
{
    if (m_pGraphics)
    {
        do
        {
            m_pSystemManager->PollGamepads();

            LogicFrame.emit(this);
        }
        while (m_pGraphics->Render());
    }
}

const std::map<uint32_t, system::input::Gamepad*>& UnicornEngine::GetGamepads() const
{
    return m_pSystemManager->GetGamepads();
}

}
