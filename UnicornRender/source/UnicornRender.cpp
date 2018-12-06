/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/UnicornRender.hpp>
#include <unicorn/video/Graphics.hpp>

#include <unicorn/system/Input.hpp>
#include <unicorn/system/Manager.hpp>

#include <unicorn/system/profiler/GamepadProfiler.hpp>
#include <unicorn/system/profiler/KeyProfiler.hpp>
#include <unicorn/system/profiler/MonitorProfiler.hpp>
#include <unicorn/system/profiler/MouseProfiler.hpp>
#include <unicorn/system/profiler/WindowProfiler.hpp>

#include <unicorn/utility/InternalLoggers.hpp>
#include <unicorn/utility/Settings.hpp>

#include <unicorn/Loggers.hpp>

#include <mule/asset/SimpleStorage.hpp>

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

UnicornRender::UnicornRender()
    : m_isInitialized(false)
    , m_pSystemManager(nullptr)
    , m_pGraphics(nullptr)
    , m_pInput(nullptr)
{
}

UnicornRender::~UnicornRender()
{
    Deinit();
}

bool UnicornRender::Init()
{
    if (m_isInitialized)
    {
        return false;
    }

    Loggers::Instance().Reinitialize();

    mule::asset::SimpleStorage::Instance();
    LOG->Info("Engine initialization started.");

    m_pSystemManager = new system::Manager();

    {
        using ProfilingMask = utility::Settings::ProfilingMask;

        ProfilingMask::MaskType const profilingMask = utility::Settings::Instance().GetProfilingMask();

        if (ProfilingMask::None != profilingMask)
        {
            if (ProfilingMask::Window & profilingMask)
            {
                s_pWindowProfiler = new system::WindowProfiler(*m_pSystemManager);
            }

            if (ProfilingMask::Monitor & profilingMask)
            {
                s_pMonitorProfiler = new system::MonitorProfiler(*m_pSystemManager);
            }

            if (ProfilingMask::Mouse & profilingMask)
            {
                s_pMouseProfiler = new system::MouseProfiler(*m_pSystemManager);
            }

            if (ProfilingMask::Key & profilingMask)
            {
                s_pKeyProfiler = new system::KeyProfiler(*m_pSystemManager);
            }

            if (ProfilingMask::Gamepad & profilingMask)
            {
                s_pGamepadProfiler = new system::GamepadProfiler(*m_pSystemManager);
            }
        }
    }

    m_pSystemManager->Init();

    m_pGraphics = new video::Graphics(*m_pSystemManager);
    m_pInput = new system::Input(*m_pSystemManager);

    if (!m_pGraphics->Init(video::DriverType::Vulkan))
    {
        Deinit();

        return false;
    }

    m_isInitialized = true;

    LOG->Info("Engine initialization finished.");

    return true;
}

void UnicornRender::Deinit()
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
        LOG->Info("Engine shutdown correctly.");
    }

    m_isInitialized = false;
}

void UnicornRender::Run()
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
