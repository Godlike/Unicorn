/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_PROFILER_KEY_PROFILER_HPP
#define UNICORN_SYSTEM_PROFILER_KEY_PROFILER_HPP

#include <unicorn/system/input/Action.hpp>
#include <unicorn/system/input/Key.hpp>
#include <unicorn/system/input/Modifier.hpp>

#include <unicorn/system/Window.hpp>

#include <cstdint>
#include <utility>

namespace unicorn
{
namespace system
{

class Manager;

/** @brief  Class that reports all window events */
class KeyProfiler
{
public:
    /** @brief  Constructs a key event profiler */
    KeyProfiler(Manager& manager);

    /** @brief  Deconstructs a key event profiler */
    ~KeyProfiler();

private:
    //! system::Manager reference
    Manager& m_systemManager;

    /** @brief  Slot invoked when window is created
     *
     *  Subscribes to all events of reported window
     *
     *  @param  pWindow newly created window
     */
    void OnWindowCreated(Window* pWindow);

    /** @brief  Slot invoked when window receives keyboard input */
    void OnWindowKeyboard(Window::KeyboardEvent const& keyboardEvent);

    /** @brief  Slot invoked when window receives unicode input */
    void OnWindowUnicode(Window* pWindow, uint32_t unicode, input::Modifier::Mask modifiers);
};

}
}

#endif /* UNICORN_SYSTEM_PROFILER_KEY_PROFILER_HPP */
