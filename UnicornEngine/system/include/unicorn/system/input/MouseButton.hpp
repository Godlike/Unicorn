/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_INPUT_MOUSE_BUTTON_HPP
#define UNICORN_SYSTEM_INPUT_MOUSE_BUTTON_HPP

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

}
}
}

#endif /* UNICORN_SYSTEM_INPUT_MOUSE_BUTTON_HPP */
