/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/system/profiler/GamepadProfiler.hpp>

#include <unicorn/system/Manager.hpp>
#include <unicorn/system/input/Gamepad.hpp>

#include <unicorn/utility/InternalLoggers.hpp>

#include <iomanip>
#include <sstream>

namespace unicorn
{
namespace system
{

GamepadProfiler::GamepadProfiler(Manager& manager)
    : m_systemManager( manager )
{
    LOG_PROFILER->Info("GamepadProfiler created");
    m_systemManager.GamepadCreated.connect(this, &GamepadProfiler::OnGamepadCreated);
}

GamepadProfiler::~GamepadProfiler()
{
    m_systemManager.GamepadCreated.disconnect(this, &GamepadProfiler::OnGamepadCreated);

    LOG_PROFILER->Info("GamepadProfiler destroyed");
}

void GamepadProfiler::OnGamepadCreated(input::Gamepad* const& pGamepad)
{
    LOG_PROFILER->Info("Gamepad[{}]: created:"
        "\r\n\t{:>8}\t{}"
        "\r\n\t{:>8}\t{}"
        "\r\n\t{:>8}\t{}"
        , pGamepad->GetId()
        , "name", pGamepad->GetName().c_str()
        , "axes", static_cast<uint32_t>(pGamepad->GetAxes().size())
        , "buttons", static_cast<uint32_t>(pGamepad->GetButtons().size()));

    pGamepad->Destroyed.connect(this, &GamepadProfiler::OnGamepadDestroyed);
    pGamepad->Updated.connect(this, &GamepadProfiler::OnGamepadUpdated);

    OnGamepadUpdated(pGamepad);
}

void GamepadProfiler::OnGamepadDestroyed(input::Gamepad* pGamepad)
{
    LOG_PROFILER->Info("Gamepad[{}]: destroyed", pGamepad->GetId());
}

void GamepadProfiler::OnGamepadUpdated(input::Gamepad* pGamepad)
{
    std::stringstream result;

    {
        const std::vector<float>& axes = pGamepad->GetAxes();

        const uint32_t size = static_cast<uint32_t>(axes.size());

        for (uint32_t i = 0; i < size; ++i)
        {
            result << "\r\n\t" << std::setw(8) << "axis " << std::setw(2) << i << "\t" << axes[i];
        }
    }

    {
        const std::vector<bool>& buttons = pGamepad->GetButtons();

        const uint32_t size = static_cast<uint32_t>(buttons.size());

        for (uint32_t i = 0; i < size; ++i)
        {
            result << "\r\n\t" << std::setw(8) << "button " << std::setw(2) << i << "\t" << buttons[i];
        }
    }

    LOG_PROFILER->Info("Gamepad[{}]: updated: {}", pGamepad->GetId(), result.str().c_str());
}

}
}
