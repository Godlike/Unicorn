/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_GRAPHICS_TEXTURE_HPP
#define UNICORN_GRAPHICS_TEXTURE_HPP

#include <unicorn/utility/SharedMacros.hpp>

#include <cstdint>
#include <string>

namespace uc
{
namespace graphics
{
class UNICORN_EXPORT Texture
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

#endif // UNICORN_GRAPHICS_TEXTURE_HPP
