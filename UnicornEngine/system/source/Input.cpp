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
    Deinit();
}

bool Input::Init()
{
    return true;
}

void Input::Deinit()
{

}

void Input::Process()
{
    m_systemManager.PollGamepads();
}

const std::map<uint32_t, input::Gamepad*>& Input::GetGamepads() const
{
    return m_systemManager.GetGamepads();
}

}
}
