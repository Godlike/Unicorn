/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_INPUT_ACTION_HPP
#define UNICORN_SYSTEM_INPUT_ACTION_HPP

#include <cstdint>

namespace unicorn
{
namespace system
{
namespace input
{

enum class Action : uint8_t
{
    Release = 0x00,
    Press,
    Repeat,

    Unknown = 0xFF
};

inline const char* Stringify(const Action& value)
{
    switch (value)
    {
        case Action::Release:
        {
            return "Release";
        }
        case Action::Press:
        {
            return "Press";
        }
        case Action::Repeat:
        {
            return "Repeat";
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

#endif /* UNICORN_SYSTEM_INPUT_ACTION_HPP */
