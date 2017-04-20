/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/system/profiler/KeyProfiler.hpp>

#include <unicorn/system/Manager.hpp>
#include <unicorn/system/Window.hpp>

#include <unicorn/system/input/Modifier.hpp>

#include <unicorn/utility/Logger.hpp>

namespace unicorn
{
namespace system
{

KeyProfiler::KeyProfiler(Manager& manager)
    : m_systemManager( manager )
{
    LOG_DEBUG("KeyProfiler created");

    m_systemManager.WindowCreated.connect(this, &KeyProfiler::OnWindowCreated);
}

KeyProfiler::~KeyProfiler()
{
    m_systemManager.WindowCreated.disconnect(this, &KeyProfiler::OnWindowCreated);

    LOG_DEBUG("KeyProfiler destroyed");
}

void KeyProfiler::OnWindowCreated(Window* pWindow)
{
    LOG_DEBUG("KeyProfiler binds to Keyboard events of Window[%d]", pWindow->GetId());

    pWindow->Keyboard.connect(this, &KeyProfiler::OnWindowKeyboard);
    pWindow->Unicode.connect(this, &KeyProfiler::OnWindowUnicode);
}

void KeyProfiler::OnWindowKeyboard(Window* pWindow, input::Key key, uint32_t scancode, input::Action action, input::Modifier::Mask modifiers)
{
    LOG_DEBUG("Window[%d]: Key input received: %s[%d] %s %s", pWindow->GetId(), input::Stringify(key), scancode, input::Stringify(action), input::Modifier::Stringify(modifiers).c_str());
}

void KeyProfiler::OnWindowUnicode(Window* pWindow, uint32_t unicode, input::Modifier::Mask modifiers)
{
    LOG_DEBUG("Window[%d]: unicode input received: %d %s", pWindow->GetId(), unicode, input::Modifier::Stringify(modifiers).c_str());
}

}
}
