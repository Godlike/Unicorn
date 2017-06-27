/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_CURSOR_SHAPE_HPP
#define UNICORN_SYSTEM_CURSOR_SHAPE_HPP

#include <cstdint>

namespace unicorn
{
namespace system
{

/** @brief  Preset cursor shapes */
enum class CursorShape : uint8_t
{
    //! Regular arrow
    Arrow,

    //! Text input cursor
    TextBeam,

    //! Basic crosshair
    Crosshair,

    //! Hand cursor
    Hand,

    //! Horizontal resize arrow
    HorizontalResize,

    //! Vertical resize arrow
    VerticalResize,

    //! Special value for cursor reset
    Reset = 0xFF
};

/** @brief  Convert cursor shape to literal string representation */
inline const char* Stringify(const CursorShape& value)
{
    switch (value)
    {
        case CursorShape::Arrow:
        {
            return "Arrow";
        }
        case CursorShape::TextBeam:
        {
            return "TextBeam";
        }
        case CursorShape::Crosshair:
        {
            return "Crosshair";
        }
        case CursorShape::Hand:
        {
            return "Hand";
        }
        case CursorShape::HorizontalResize:
        {
            return "HorizontalResize";
        }
        case CursorShape::VerticalResize:
        {
            return "VerticalResize";
        }
        case CursorShape::Reset:
        {
            return "Reset";
        }
        default:
        {
            return "Unknown";
        }
    }
}

}
}

#endif // UNICORN_SYSTEM_CURSOR_SHAPE_HPP
