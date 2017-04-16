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

/** @brief  Keyboard input modifiers  */
struct Modifier
{
    //! Alias for a masked type
    typedef uint8_t Mask;

    //! Alias for no modifiers
    static const Mask None = 0x00;

    //! @{
    //! Modifier flags
    static const Mask Shift = 0x01;
    static const Mask Ctrl = 0x02;
    static const Mask Alt = 0x04;
    static const Mask Super = 0x08;
    //! @}

    //! Alias for unknown modifiers
    static const Mask Unknown = 0xFF ^ (Shift | Ctrl | Alt | Super);

    /** @brief  Convert masked value to string
     *
     *  @param  value   masked value
     *
     *  @return string representation
     */
    inline static std::string Stringify(Mask value)
    {
        std::string result;

        if (Unknown != value)
        {
            if (Shift & value)
            {
                result.append(" | Shift");
            }

            if (Ctrl & value)
            {
                result.append(" | Ctrl");
            }

            if (Alt & value)
            {
                result.append(" | Alt");
            }

            if (Super & value)
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
