/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_INPUT_JOYSTICK_HPP
#define UNICORN_SYSTEM_INPUT_JOYSTICK_HPP

#include <cstdint>

namespace unicorn
{
namespace system
{
namespace input
{

enum class Joystick : uint8_t
{
    Joystick0   = 0x00,
    Joystick1,
    Joystick2,
    Joystick3,
    Joystick4,
    Joystick5,
    Joystick6,
    Joystick7,
    Joystick8,
    Joystick9,
    Joystick10,
    Joystick11,
    Joystick12,
    Joystick13,
    Joystick14,
    Joystick15,
    Joystick16,

    Unknown     = 0xFF
};

enum class JoystickState : uint8_t
{
    Connected       = 0x00,
    Disconnected    = 0x01,

    Unknown         = 0xFF
};

}
}
}

#endif /* UNICORN_SYSTEM_INPUT_JOYSTICK_HPP */
