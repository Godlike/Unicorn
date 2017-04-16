/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_INPUT_MODIFIER_HPP
#define UNICORN_SYSTEM_INPUT_MODIFIER_HPP

#include <cstdint>
#include <string>

namespace unicorn
{
namespace system
{
namespace input
{

struct Modifier
{
    static const uint8_t None = 0x00;

    static const uint8_t Shift = 0x01;
    static const uint8_t Ctrl = 0x02;
    static const uint8_t Alt = 0x04;
    static const uint8_t Super = 0x08;

    static const uint8_t Unknown = 0xFF;

    inline static std::string Stringify(uint32_t value)
    {
        std::string result;

        if (Unknown != value)
        {
            if (value & Shift)
            {
                result.append(" | Shift");
            }

            if (value & Ctrl)
            {
                result.append(" | Ctrl");
            }

            if (value & Alt)
            {
                result.append(" | Alt");
            }

            if (value & Super)
            {
                result.append(" | Super");
            }

            if (result.empty())
            {
                result = "[No modifiers]";
            }
            else
            {
                result = result.substr(3);
            }
        }
        else
        {
            result = "[Unknown modifiers]";
        }

        return result;
    }
};

}
}
}

#endif /* UNICORN_SYSTEM_INPUT_MODIFIER_HPP */
