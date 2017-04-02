/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_WINDOW_MANAGER_PROFILERS_WINDOW_PROFILER_HPP
#define UNICORN_WINDOW_MANAGER_PROFILERS_WINDOW_PROFILER_HPP

#include <cstdint>
#include <utility>

namespace unicorn
{
namespace WindowManager
{

class Hub;
class Window;

class WindowProfiler
{
public:
    WindowProfiler(Hub& hub);
    ~WindowProfiler();

private:
    Hub& m_hub;

    void OnWindowCreated(Window* pWindow);

    void OnWindowDestroyed(Window* pWindow);
    void OnWindowPositionChanged(Window* pWindow, std::pair<int32_t, int32_t> position);
    void OnWindowSizeChanged(Window* pWindow, std::pair<int32_t, int32_t> size);
    void OnWindowClose(Window* pWindow);
    void OnWindowContentRefresh(Window* pWindow);
    void OnWindowFocused(Window* pWindow, bool flag);
    void OnWindowMinimized(Window* pWindow, bool flag);
    void OnWindowMaximized(Window* pWindow, bool flag);
    void OnWindowFramebufferResized(Window* pWindow, std::pair<int32_t, int32_t> size);
};

}
}

#endif /* UNICORN_WINDOW_MANAGER_PROFILERS_WINDOW_PROFILER_HPP */
