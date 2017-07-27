/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_SPRITE_HPP
#define UNICORN_VIDEO_SPRITE_HPP

#include <unicorn/video/Texture.hpp>
#include <unicorn/video/geometry/Model.hpp>
#include <unicorn/utility/SharedMacros.hpp>

#include <cstdint>

namespace unicorn
{
namespace video
{
class Sprite
{
public:
    UNICORN_EXPORT Sprite(const Texture& texture, uint32_t x, uint32_t y, uint32_t width, uint32_t height);
private:
    uint32_t m_x;
    uint32_t m_y;
    uint32_t m_width;
    uint32_t m_height;
    Model* m_model;
};
}
}

#endif // UNICORN_VIDEO_SPRITE_HPP
