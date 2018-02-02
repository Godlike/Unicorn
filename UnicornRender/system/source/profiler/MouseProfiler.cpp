/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/system/profiler/MouseProfiler.hpp>

#include <unicorn/system/Manager.hpp>

#include <unicorn/system/input/Modifier.hpp>

#include <mule/Logger.hpp>

namespace unicorn
{
namespace system
{

MouseProfiler::MouseProfiler(Manager& manager)
    : m_systemManager( manager )
{
    LOG_INFO("MouseProfiler created");

    m_systemManager.WindowCreated.connect(this, &MouseProfiler::OnWindowCreated);
}

MouseProfiler::~MouseProfiler()
{
    m_systemManager.WindowCreated.disconnect(this, &MouseProfiler::OnWindowCreated);

    LOG_INFO("MouseProfiler destroyed");
}

void MouseProfiler::OnWindowCreated(Window* pWindow)
{
    LOG_INFO("MouseProfiler binds to mouse events of Window[%d]", pWindow->GetId());

    pWindow->MouseButton.connect(this, &MouseProfiler::OnWindowMouseButton);
    pWindow->MousePosition.connect(this, &MouseProfiler::OnWindowMousePosition);
    pWindow->MouseEnter.connect(this, &MouseProfiler::OnWindowMouseEnter);
    pWindow->Scroll.connect(this, &MouseProfiler::OnWindowScroll);
}

void MouseProfiler::OnWindowMouseButton(Window::MouseButtonEvent const& mouseButtonEvent)
{
    Window* const& pWindow = mouseButtonEvent.pWindow;
    input::MouseButton const& button = mouseButtonEvent.button;
    input::Action const& action = mouseButtonEvent.action;
    input::Modifier::Mask const& modifiers = mouseButtonEvent.modifiers;

    LOG_INFO("Window[%d]: mouse input received: %s %s %s", pWindow->GetId(), input::Stringify(button), input::Stringify(action), input::Modifier::Stringify(modifiers).c_str());
}

void MouseProfiler::OnWindowMousePosition(Window* pWindow, std::pair<double, double> coords)
{
    LOG_INFO("Window[%d]: mouse position changed to %f:%f", pWindow->GetId(), coords.first, coords.second);
}

void MouseProfiler::OnWindowMouseEnter(Window* pWindow, bool entered)
{
    LOG_INFO("Window[%d]: mouse %s the window", pWindow->GetId(), entered ? "entered" : "left");
}

void MouseProfiler::OnWindowScroll(Window* pWindow, std::pair<double, double> coords)
{
    LOG_INFO("Window[%d]: scroll received %f:%f", pWindow->GetId(), coords.first, coords.second);
}

}
}
