/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_INPUT_KEY_HPP
#define UNICORN_SYSTEM_INPUT_KEY_HPP

namespace unicorn
{
namespace system
{
namespace input
{

enum class Key : uint8_t
{
    // Printable
    Space           = 0x00,
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

}
}
}

#endif /* UNICORN_SYSTEM_INPUT_KEY_HPP */
