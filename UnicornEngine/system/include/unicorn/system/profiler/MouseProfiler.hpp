/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_PROFILER_MOUSE_PROFILER_HPP
#define UNICORN_SYSTEM_PROFILER_MOUSE_PROFILER_HPP

#include <unicorn/system/input/MouseButton.hpp>
#include <unicorn/system/input/Action.hpp>
#include <unicorn/system/input/Key.hpp>
#include <unicorn/system/input/Joystick.hpp>

#include <cstdint>
#include <utility>

namespace unicorn
{
namespace system
{

class Manager;
class Window;

/** @brief  Class that reports all window events */
class MouseProfiler
{
public:
    /** @brief  Constructs a mouse event profiler */
    MouseProfiler(Manager& manager);

    /** @brief  Deconstructs a mouse event profiler */
    ~MouseProfiler();

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

    /** @brief  Slot invoked when window receives mouse button input */
    void OnWindowMouseButton(Window* pWindow, input::MouseButton button, input::Action action, uint32_t modifiers);

    /** @brief  Slot invoked when mouse position is changed within a window */
    void OnWindowMousePosition(Window* pWindow, std::pair<double, double> coords);

    /** @brief  Slot invoked when mouse enters or leaves window */
    void OnWindowMouseEnter(Window* pWindow, bool entered);
};

}
}

#endif /* UNICORN_SYSTEM_PROFILER_MOUSE_PROFILER_HPP */
