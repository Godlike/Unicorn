/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_PROFILER_GAMEPAD_PROFILER_HPP
#define UNICORN_SYSTEM_PROFILER_GAMEPAD_PROFILER_HPP

#include <unicorn/system/input/Gamepad.hpp>

#include <cstdint>
#include <utility>

namespace unicorn
{
namespace system
{

class Manager;

/** @brief  Class that reports all gamepad events */
class GamepadProfiler
{
public:
    /** @brief  Constructs a gamepad event profiler */
    GamepadProfiler(Manager& manager);

    /** @brief  Deconstructs a gamepad event profiler */
    ~GamepadProfiler();

private:
    //! system::Manager reference
    Manager& m_systemManager;

    /** @brief  Slot invoked when gamepad is created
     *
     *  Subscribes to all events of reported gamepad
     *
     *  @param  pGamepad    newly created gamepad
     */
    void OnGamepadCreated(input::Gamepad* const& pGamepad);

    /** @brief  Slot invoken when gamepad is destroyed */
    void OnGamepadDestroyed(input::Gamepad* pGamepad);

    /** @brief  Slot invoken when gamepad data is updated */
    void OnGamepadUpdated(input::Gamepad* pGamepad);
};

}
}

#endif /* UNICORN_SYSTEM_PROFILER_GAMEPAD_PROFILER_HPP */
