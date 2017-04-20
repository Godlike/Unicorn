/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_INPUT_GAMEPAD_STATE_HPP
#define UNICORN_SYSTEM_INPUT_GAMEPAD_STATE_HPP

#include <cstdint>

namespace unicorn
{
namespace system
{
namespace input
{

enum class GamepadState : uint8_t
{
    Connected       = 0x00,
    Disconnected    = 0x01,

    Unknown         = 0xFF
};

}
}
}

#endif /* UNICORN_SYSTEM_INPUT_GAMEPAD_STATE_HPP */
