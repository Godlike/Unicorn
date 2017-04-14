/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_WINDOW_ATTRIBUTE_HPP
#define UNICORN_SYSTEM_WINDOW_ATTRIBUTE_HPP

#include <cstdint>

namespace unicorn
{
namespace system
{

/** @brief  Window attributes */
enum class WindowAttribute : uint32_t
{
    // Window related
    Decorated,
    Floating,
    Focused,
    Maximized,
    Minimized,
    Resizable,
    Visible,

    // Framebuffer related
    Samples,

    // Context related
    ClientAPI,
    ContextCreationAPI,
    ContextVersionMajor,
    ContextVersionMinor,
};

}
}

#endif /* UNICORN_SYSTEM_WINDOW_ATTRIBUTE_HPP */
