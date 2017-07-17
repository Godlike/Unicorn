/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/system/Input.hpp>

#include <unicorn/system/Manager.hpp>

namespace unicorn
{
namespace system
{

Input::Input(system::Manager& manager)
    : GamepadCreated(manager.GamepadCreated)
    , m_systemManager(manager)
{
}

Input::~Input()
{

}

std::string Input::GetClipboard() const
{
    return m_systemManager.GetClipboard();
}

void Input::SetClipboard(const std::string& data) const
{
    m_systemManager.SetClipboard(data);
}

const std::map<uint32_t, input::Gamepad*>& Input::GetGamepads() const
{
    return m_systemManager.GetGamepads();
}

void Input::Process()
{
    m_systemManager.PollGamepads();
    m_systemManager.PollWindows();
}

}
}
