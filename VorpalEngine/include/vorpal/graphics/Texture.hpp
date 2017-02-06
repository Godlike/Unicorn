/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_GRAPHICS_TEXTURE_HPP
#define VORPAL_GRAPHICS_TEXTURE_HPP

#include <vorpal/utility/SharedMacros.hpp>

#include <cstdint>
#include <string>

namespace vp
{
namespace graphics
{
class VORPAL_EXPORT Texture
{
public:
    bool LoadFromFile(const std::string& path);

private:
    int m_width;
    int m_height;
    int m_channels;
};
}
}

#endif // VORPAL_GRAPHICS_TEXTURE_HPP
