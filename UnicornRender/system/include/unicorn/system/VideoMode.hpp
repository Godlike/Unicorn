/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_VIDEO_MODE_HPP
#define UNICORN_SYSTEM_VIDEO_MODE_HPP

#include <array>
#include <cstdint>

namespace unicorn
{
namespace system
{

/** @brief  Represents monitor's video modes */
struct VideoMode
{
    /** @brief  Equal comparison operator */
    bool operator==(const VideoMode& other) const
    {
        return width == other.width &&
            height == other.height &&
            rgbBitDepth == other.rgbBitDepth &&
            refreshRate == other.refreshRate;
    }

    //! Width in screen coordinates
    int32_t width;

    //! Height in screen coordinates
    int32_t height;

    //! Bit depths of red, green and blue channels
    std::array<int32_t, 3> rgbBitDepth;

    //! Refresh rate in Hz
    int32_t refreshRate;
};

}
}

#endif /* UNICORN_SYSTEM_VIDEO_MODE_HPP */
