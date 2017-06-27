/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_PROFILER_WINDOW_PROFILER_HPP
#define UNICORN_SYSTEM_PROFILER_WINDOW_PROFILER_HPP

#include <cstdint>
#include <utility>

namespace unicorn
{
namespace system
{

class Manager;
class Window;

/** @brief  Class that reports all window events */
class WindowProfiler
{
public:
    /** @brief  Constructs a window profiler */
    WindowProfiler(Manager& manager);

    /** @brief  Deconstructs a window profiler */
    ~WindowProfiler();

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

    /** @brief  Slot invoked when window is destroyed */
    void OnWindowDestroyed(Window* pWindow);

    /** @brief  Slot invoked when window position is changed */
    void OnWindowPositionChanged(Window* pWindow, std::pair<int32_t, int32_t> position);

    /** @brief  Slot invoked when window size is changed */
    void OnWindowSizeChanged(Window* pWindow, std::pair<int32_t, int32_t> size);

    /** @brief  Slot invoked when window is about to be closed */
    void OnWindowClose(Window* pWindow);

    /** @brief  Slot invoked when window content should be refreshed */
    void OnWindowContentRefresh(Window* pWindow);

    /** @brief  Slot invoked when window focus is changed */
    void OnWindowFocused(Window* pWindow, bool flag);

    /** @brief  Slot invoked when window minimized stae is changed */
    void OnWindowMinimized(Window* pWindow, bool flag);

    /** @brief  Slot invoked when window maximized state is changed */
    void OnWindowMaximized(Window* pWindow, bool flag);

    /** @brief  Slot invoked when window framebuffer size is changed */
    void OnWindowFramebufferResized(Window* pWindow, std::pair<int32_t, int32_t> size);
};

}
}

#endif /* UNICORN_SYSTEM_PROFILER_WINDOW_PROFILER_HPP */
