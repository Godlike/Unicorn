/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_INPUT_KEY_HPP
#define UNICORN_SYSTEM_INPUT_KEY_HPP

#include <cstdint>

namespace unicorn
{
namespace system
{
namespace input
{

/** @brief  Detectable key codes */
enum class Key : uint8_t
{
    Unrecognized    = 0x00,

    // Printable
    Space,
    Apostrophe,
    Comma,
    Minus,
    Period,
    Slash,
    Normal_0,
    Normal_1,
    Normal_2,
    Normal_3,
    Normal_4,
    Normal_5,
    Normal_6,
    Normal_7,
    Normal_8,
    Normal_9,
    Semicolon,
    Equal,
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    LeftBracket,
    Backslash,
    RightBracket,
    GraveAccent,
    World_1,
    World_2,

    // Functional
    Escape,
    Enter,
    Tab,
    Backspace,
    Insert,
    Delete,
    Right,
    Left,
    Down,
    Up,
    PageUp,
    PageDown,
    Home,
    End,
    CapsLock,
    ScrollLock,
    NumLock,
    PrintScreen,
    Pause,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    F13,
    F14,
    F15,
    F16,
    F17,
    F18,
    F19,
    F20,
    F21,
    F22,
    F23,
    F24,
    F25,
    Num_0,
    Num_1,
    Num_2,
    Num_3,
    Num_4,
    Num_5,
    Num_6,
    Num_7,
    Num_8,
    Num_9,
    Num_Decimal,
    Num_Divide,
    Num_Multiply,
    Num_Subtract,
    Num_Add,
    Num_Enter,
    Num_Equal,
    LeftShift,
    LeftControl,
    LeftAlt,
    LeftSuper,
    RightShift,
    RightControl,
    RightAlt,
    RightSuper,
    Menu,

    Unknown         = 0xFF
};

/** @brief  Convert key value to literal string representation */
inline const char* Stringify(const Key& value)
{
    switch (value)
    {
        case Key::Unrecognized:
        {
            return "Unrecognized";
        }
        case Key::Space:
        {
            return "Space";
        }
        case Key::Apostrophe:
        {
            return "Apostrophe";
        }
        case Key::Comma:
        {
            return "Comma";
        }
        case Key::Minus:
        {
            return "Minus";
        }
        case Key::Period:
        {
            return "Period";
        }
        case Key::Slash:
        {
            return "Slash";
        }
        case Key::Normal_0:
        {
            return "0";
        }
        case Key::Normal_1:
        {
            return "1";
        }
        case Key::Normal_2:
        {
            return "2";
        }
        case Key::Normal_3:
        {
            return "3";
        }
        case Key::Normal_4:
        {
            return "4";
        }
        case Key::Normal_5:
        {
            return "5";
        }
        case Key::Normal_6:
        {
            return "6";
        }
        case Key::Normal_7:
        {
            return "7";
        }
        case Key::Normal_8:
        {
            return "8";
        }
        case Key::Normal_9:
        {
            return "9";
        }
        case Key::Semicolon:
        {
            return "Semicolon";
        }
        case Key::Equal:
        {
            return "Equal";
        }
        case Key::A:
        {
            return "A";
        }
        case Key::B:
        {
            return "B";
        }
        case Key::C:
        {
            return "C";
        }
        case Key::D:
        {
            return "D";
        }
        case Key::E:
        {
            return "E";
        }
        case Key::F:
        {
            return "F";
        }
        case Key::G:
        {
            return "G";
        }
        case Key::H:
        {
            return "H";
        }
        case Key::I:
        {
            return "I";
        }
        case Key::J:
        {
            return "J";
        }
        case Key::K:
        {
            return "K";
        }
        case Key::L:
        {
            return "L";
        }
        case Key::M:
        {
            return "M";
        }
        case Key::N:
        {
            return "N";
        }
        case Key::O:
        {
            return "O";
        }
        case Key::P:
        {
            return "P";
        }
        case Key::Q:
        {
            return "Q";
        }
        case Key::R:
        {
            return "R";
        }
        case Key::S:
        {
            return "S";
        }
        case Key::T:
        {
            return "T";
        }
        case Key::U:
        {
            return "U";
        }
        case Key::V:
        {
            return "V";
        }
        case Key::W:
        {
            return "W";
        }
        case Key::X:
        {
            return "X";
        }
        case Key::Y:
        {
            return "Y";
        }
        case Key::Z:
        {
            return "Z";
        }
        case Key::LeftBracket:
        {
            return "LeftBracket";
        }
        case Key::Backslash:
        {
            return "Backslash";
        }
        case Key::RightBracket:
        {
            return "RightBracket";
        }
        case Key::GraveAccent:
        {
            return "GraveAccent";
        }
        case Key::World_1:
        {
            return "World_1";
        }
        case Key::World_2:
        {
            return "World_2";
        }
        case Key::Escape:
        {
            return "Escape";
        }
        case Key::Enter:
        {
            return "Enter";
        }
        case Key::Tab:
        {
            return "Tab";
        }
        case Key::Backspace:
        {
            return "Backspace";
        }
        case Key::Insert:
        {
            return "Insert";
        }
        case Key::Delete:
        {
            return "Delete";
        }
        case Key::Right:
        {
            return "Right";
        }
        case Key::Left:
        {
            return "Left";
        }
        case Key::Down:
        {
            return "Down";
        }
        case Key::Up:
        {
            return "Up";
        }
        case Key::PageUp:
        {
            return "PageUp";
        }
        case Key::PageDown:
        {
            return "PageDown";
        }
        case Key::Home:
        {
            return "Home";
        }
        case Key::End:
        {
            return "End";
        }
        case Key::CapsLock:
        {
            return "CapsLock";
        }
        case Key::ScrollLock:
        {
            return "ScrollLock";
        }
        case Key::NumLock:
        {
            return "NumLock";
        }
        case Key::PrintScreen:
        {
            return "PrintScreen";
        }
        case Key::Pause:
        {
            return "Pause";
        }
        case Key::F1:
        {
            return "F1";
        }
        case Key::F2:
        {
            return "F2";
        }
        case Key::F3:
        {
            return "F3";
        }
        case Key::F4:
        {
            return "F4";
        }
        case Key::F5:
        {
            return "F5";
        }
        case Key::F6:
        {
            return "F6";
        }
        case Key::F7:
        {
            return "F7";
        }
        case Key::F8:
        {
            return "F8";
        }
        case Key::F9:
        {
            return "F9";
        }
        case Key::F10:
        {
            return "F10";
        }
        case Key::F11:
        {
            return "F11";
        }
        case Key::F12:
        {
            return "F12";
        }
        case Key::F13:
        {
            return "F13";
        }
        case Key::F14:
        {
            return "F14";
        }
        case Key::F15:
        {
            return "F15";
        }
        case Key::F16:
        {
            return "F16";
        }
        case Key::F17:
        {
            return "F17";
        }
        case Key::F18:
        {
            return "F18";
        }
        case Key::F19:
        {
            return "F19";
        }
        case Key::F20:
        {
            return "F20";
        }
        case Key::F21:
        {
            return "F21";
        }
        case Key::F22:
        {
            return "F22";
        }
        case Key::F23:
        {
            return "F23";
        }
        case Key::F24:
        {
            return "F24";
        }
        case Key::F25:
        {
            return "F25";
        }
        case Key::Num_0:
        {
            return "Num_0";
        }
        case Key::Num_1:
        {
            return "Num_1";
        }
        case Key::Num_2:
        {
            return "Num_2";
        }
        case Key::Num_3:
        {
            return "Num_3";
        }
        case Key::Num_4:
        {
            return "Num_4";
        }
        case Key::Num_5:
        {
            return "Num_5";
        }
        case Key::Num_6:
        {
            return "Num_6";
        }
        case Key::Num_7:
        {
            return "Num_7";
        }
        case Key::Num_8:
        {
            return "Num_8";
        }
        case Key::Num_9:
        {
            return "Num_9";
        }
        case Key::Num_Decimal:
        {
            return "Num_Decimal";
        }
        case Key::Num_Divide:
        {
            return "Num_Divide";
        }
        case Key::Num_Multiply:
        {
            return "Num_Multiply";
        }
        case Key::Num_Subtract:
        {
            return "Num_Subtract";
        }
        case Key::Num_Add:
        {
            return "Num_Add";
        }
        case Key::Num_Enter:
        {
            return "Num_Enter";
        }
        case Key::Num_Equal:
        {
            return "Num_Equal";
        }
        case Key::LeftShift:
        {
            return "LeftShift";
        }
        case Key::LeftControl:
        {
            return "LeftControl";
        }
        case Key::LeftAlt:
        {
            return "LeftAlt";
        }
        case Key::LeftSuper:
        {
            return "LeftSuper";
        }
        case Key::RightShift:
        {
            return "RightShift";
        }
        case Key::RightControl:
        {
            return "RightControl";
        }
        case Key::RightAlt:
        {
            return "RightAlt";
        }
        case Key::RightSuper:
        {
            return "RightSuper";
        }
        case Key::Menu:
        {
            return "Menu";
        }
        default:
        {
            return "[unknown]";
        }
    }
}

}
}
}

#endif /* UNICORN_SYSTEM_INPUT_KEY_HPP */
