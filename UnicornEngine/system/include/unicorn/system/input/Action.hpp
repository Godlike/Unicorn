/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_INPUT_ACTION_HPP
#define UNICORN_SYSTEM_INPUT_ACTION_HPP

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

}
}
}

#endif /* UNICORN_SYSTEM_INPUT_ACTION_HPP */
