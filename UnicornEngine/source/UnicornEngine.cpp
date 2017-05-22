/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/UnicornEngine.hpp>
#include <unicorn/utility/Logger.hpp>
#include <unicorn/video/Graphics.hpp>
#include <unicorn/utility/asset/SimpleStorage.hpp>

#include <unicorn/system/Input.hpp>
#include <unicorn/system/Manager.hpp>

#include <unicorn/system/profiler/GamepadProfiler.hpp>
#include <unicorn/system/profiler/KeyProfiler.hpp>
#include <unicorn/system/profiler/MonitorProfiler.hpp>
#include <unicorn/system/profiler/MouseProfiler.hpp>
#include <unicorn/system/profiler/WindowProfiler.hpp>

namespace
{
//! Pointer to the window profiler
static unicorn::system::WindowProfiler* s_pWindowProfiler = nullptr;

//! Pointer to the monitor profiler
static unicorn::system::MonitorProfiler* s_pMonitorProfiler = nullptr;

//! Pointer to the mouse profiler
static unicorn::system::MouseProfiler* s_pMouseProfiler = nullptr;

//! Pointer to the key profiler
static unicorn::system::KeyProfiler* s_pKeyProfiler = nullptr;

//! Pointer to the gamepad profiler
static unicorn::system::GamepadProfiler* s_pGamepadProfiler = nullptr;
}

namespace unicorn
{
UnicornEngine::UnicornEngine()
    : m_isInitialized(false)
      , m_pSystemManager(nullptr)
      , m_pGraphics(nullptr)
      , m_pInput(nullptr)
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

    s_pWindowProfiler = new system::WindowProfiler(*m_pSystemManager);
    s_pMonitorProfiler = new system::MonitorProfiler(*m_pSystemManager);
    s_pMouseProfiler = new system::MouseProfiler(*m_pSystemManager);
    s_pKeyProfiler = new system::KeyProfiler(*m_pSystemManager);
    s_pGamepadProfiler = new system::GamepadProfiler(*m_pSystemManager);

    m_pSystemManager->Init();

    m_pGraphics = new video::Graphics(*m_pSystemManager);
    m_pInput = new system::Input(*m_pSystemManager);

    if (!m_pGraphics->Init(video::DriverType::Vulkan))
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

        if (s_pGamepadProfiler)
        {
            delete s_pGamepadProfiler;

            s_pGamepadProfiler = nullptr;
        }

        if (s_pKeyProfiler)
        {
            delete s_pKeyProfiler;

            s_pKeyProfiler = nullptr;
        }

        if (s_pMouseProfiler)
        {
            delete s_pMouseProfiler;

            s_pMouseProfiler = nullptr;
        }

        if (s_pMonitorProfiler)
        {
            delete s_pMonitorProfiler;

            s_pMonitorProfiler = nullptr;
        }

        if (s_pWindowProfiler)
        {
            delete s_pWindowProfiler;

            s_pWindowProfiler = nullptr;
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
            m_pSystemManager->PollEvents();

            m_pInput->Process();

            LogicFrame.emit(this);
        } while (m_pGraphics->Render());
    }
}
}
