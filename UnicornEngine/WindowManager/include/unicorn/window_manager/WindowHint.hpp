/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_WINDOW_MANAGER_WINDOW_HINT_HPP
#define UNICORN_WINDOW_MANAGER_WINDOW_HINT_HPP

#include <cstdint>

namespace unicorn
{
namespace WindowManager
{

/** @brief  Window creation hints */
enum class WindowHint : uint32_t
{
    // Window related
    AutoMinimize,
    Decorated,
    Floating,
    Focused,
    Maximized,
    Resizable,
    Visible,

    // Framebuffer related
    Doublebuffer,
    Stereo,
    Samples,

    // Monitor related
    RefreshRate,

    // Context related
    ClientAPI,
    ContextCreationAPI,
    ContextVersionMajor,
    ContextVersionMinor,
};

}
}

#endif /* UNICORN_WINDOW_MANAGER_WINDOW_HINT_HPP */
