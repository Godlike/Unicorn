/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_INPUT_MOUSE_BUTTON_HPP
#define UNICORN_SYSTEM_INPUT_MOUSE_BUTTON_HPP

#include <cstdint>

namespace unicorn
{
namespace system
{
namespace input
{

enum class MouseButton : uint8_t
{
    Mouse0      = 0x00,
    Mouse1,
    Mouse2,
    Mouse3,
    Mouse4,
    Mouse5,
    Mouse6,
    Mouse7,
    Mouse8,

    MouseLeft   = Mouse1,
    MouseRight  = Mouse2,
    MouseMiddle = Mouse3,

    Unknown     = 0xFF
};

inline const char* Stringify(const MouseButton& value)
{
    switch (value)
    {
        case MouseButton::Mouse0:
        {
            return "Mouse0";
        }
        case MouseButton::Mouse1:
        {
            return "Mouse1";
        }
        case MouseButton::Mouse2:
        {
            return "Mouse2";
        }
        case MouseButton::Mouse3:
        {
            return "Mouse3";
        }
        case MouseButton::Mouse4:
        {
            return "Mouse4";
        }
        case MouseButton::Mouse5:
        {
            return "Mouse5";
        }
        case MouseButton::Mouse6:
        {
            return "Mouse6";
        }
        case MouseButton::Mouse7:
        {
            return "Mouse7";
        }
        case MouseButton::Mouse8:
        {
            return "Mouse8";
        }
        default:
        {
            return "Unknown";
        }
    }
}

}
}
}

#endif /* UNICORN_SYSTEM_INPUT_MOUSE_BUTTON_HPP */
