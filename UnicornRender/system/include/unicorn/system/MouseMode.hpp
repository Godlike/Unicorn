/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_MOUSE_MODE_HPP
#define UNICORN_SYSTEM_MOUSE_MODE_HPP

#include <cstdint>

namespace unicorn
{
namespace system
{

/** @brief  Mouse interaction modes */
enum class MouseMode : uint8_t
{
    //! Visible mouse cursor that can travel in and out of the window
    Normal,

    //! Invisible mouse cursor that can travel in and out of the window
    Hidden,

    //! Invisible mouse cursor that cannot leave the window
    Captured
};

/** @brief  Convert mouse interaction mode to literal string representation */
inline const char* Stringify(const MouseMode& value)
{
    switch (value)
    {
        case MouseMode::Normal:
        {
            return "Normal";
        }
        case MouseMode::Hidden:
        {
            return "Hidden";
        }
        case MouseMode::Captured:
        {
            return "Captured";
        }
        default:
        {
            return "Unknown";
        }
    }
}

}
}

#endif // UNICORN_SYSTEM_MOUSE_MODE_HPP
