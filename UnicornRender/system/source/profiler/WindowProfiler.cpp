/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/system/profiler/WindowProfiler.hpp>

#include <unicorn/system/Manager.hpp>
#include <unicorn/system/Window.hpp>

#include <unicorn/utility/InternalLoggers.hpp>

namespace unicorn
{
namespace system
{

WindowProfiler::WindowProfiler(Manager& manager)
    : m_systemManager( manager )
{
    LOG_PROFILER->Info("WindowProfiler created");
    m_systemManager.WindowCreated.connect(this, &WindowProfiler::OnWindowCreated);
}

WindowProfiler::~WindowProfiler()
{
    m_systemManager.WindowCreated.disconnect(this, &WindowProfiler::OnWindowCreated);

    LOG_PROFILER->Info("WindowProfiler destroyed");
}

void WindowProfiler::OnWindowCreated(Window* pWindow)
{
    const std::pair<int32_t, int32_t>& size = pWindow->GetSize();
    const std::pair<int32_t, int32_t>& position = pWindow->GetPosition();
    Monitor* pMonitor = m_systemManager.GetWindowMonitor(*pWindow);

    LOG_PROFILER->Info("Window[{}]: created:"
        "\r\n\t{:>8}\t{}"
        "\r\n\t{:>8}\t{}x{}"
        "\r\n\t{:>8}\t{}:{}"
        "\r\n\t{:>8}\t{}"
        , pWindow->GetId()
        , "name", pWindow->GetName().c_str()
        , "size", size.first, size.second
        , "position", position.first, position.second
        , "monitor", pMonitor != nullptr ? pMonitor->GetId() : 0);

    pWindow->Destroyed.connect(this, &WindowProfiler::OnWindowDestroyed);

    pWindow->PositionChanged.connect(this, &WindowProfiler::OnWindowPositionChanged);
    pWindow->SizeChanged.connect(this, &WindowProfiler::OnWindowSizeChanged);
    pWindow->Close.connect(this, &WindowProfiler::OnWindowClose);
    pWindow->ContentRefresh.connect(this, &WindowProfiler::OnWindowContentRefresh);
    pWindow->Focused.connect(this, &WindowProfiler::OnWindowFocused);
    pWindow->Minimized.connect(this, &WindowProfiler::OnWindowMinimized);
    pWindow->Maximized.connect(this, &WindowProfiler::OnWindowMaximized);
    pWindow->CursorShapeChanged.connect(this, &WindowProfiler::OnCursorShapeChanged);
    pWindow->MouseModeChanged.connect(this, &WindowProfiler::OnMouseModeChanged);
    pWindow->FramebufferResized.connect(this, &WindowProfiler::OnWindowFramebufferResized);
}

void WindowProfiler::OnWindowDestroyed(Window* pWindow)
{
    LOG_PROFILER->Info("Window[{}]: destroyed", pWindow->GetId());
}

void WindowProfiler::OnWindowPositionChanged(Window* pWindow, std::pair<int32_t, int32_t> position)
{
    LOG_PROFILER->Info("Window[{}]: position changed to {}:{}", pWindow->GetId(), position.first, position.second);
}

void WindowProfiler::OnWindowSizeChanged(Window* pWindow, std::pair<int32_t, int32_t> size)
{
    LOG_PROFILER->Info("Window[{}]: size changed to {}:{}", pWindow->GetId(), size.first, size.second);
}

void WindowProfiler::OnWindowClose(Window* pWindow)
{
    LOG_PROFILER->Info("Window[{}]: closed", pWindow->GetId());
}

void WindowProfiler::OnWindowContentRefresh(Window* pWindow)
{
    LOG_PROFILER->Info("Window[{}]: content refresh requested", pWindow->GetId());
}

void WindowProfiler::OnWindowFocused(Window* pWindow, bool flag)
{
    LOG_PROFILER->Info("Window[{}]: focus {}", pWindow->GetId(), flag ? "gained" : "lost");
}

void WindowProfiler::OnWindowMinimized(Window* pWindow, bool flag)
{
    LOG_PROFILER->Info("Window[{}]: minimize event: {}", pWindow->GetId(), flag ? "iconify" : "restore");
}

void WindowProfiler::OnWindowMaximized(Window* pWindow, bool flag)
{
    LOG_PROFILER->Info("Window[{}]: maximize event: {}", pWindow->GetId(), flag ? "maximize" : "restore");
}

void WindowProfiler::OnCursorShapeChanged(Window* pWindow, CursorShape shape)
{
    LOG_PROFILER->Info("Window[{}]: cursor shape is changed to {}", pWindow->GetId(), Stringify(shape));
}

void WindowProfiler::OnMouseModeChanged(Window* pWindow, MouseMode mode)
{
    LOG_PROFILER->Info("Window[{}]: mouse mode is changed to {}", pWindow->GetId(), Stringify(mode));
}

void WindowProfiler::OnWindowFramebufferResized(Window* pWindow, std::pair<int32_t, int32_t> size)
{
    LOG_PROFILER->Info("Window[{}]: framebuffer size changed to {}:{}", pWindow->GetId(), size.first, size.second);
}

}
}
