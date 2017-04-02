/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_WINDOW_MANAGER_VIDEO_MODE_HPP
#define UNICORN_WINDOW_MANAGER_VIDEO_MODE_HPP

#include <array>
#include <cstdint>

namespace unicorn
{
namespace WindowManager
{

/** @brief  Represents monitor's video modes */
struct VideoMode
{
    //! Width in screen coordinates
    int32_t width;

    //! Height in screen coordinates
    int32_t height;

    //! Bit depths of red, green and blue channels
    std::array<int32_t, 3> rgbBitDepth;

    //! Refresh rate in Hz
    int32_t refreshRate;

    //! Flag describing if this video mode is currently used
    bool isCurrent;
};

}
}

#endif /* UNICORN_WINDOW_MANAGER_VIDEO_MODE_HPP */
