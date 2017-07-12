/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_TEXTURE_HPP
#define UNICORN_VIDEO_TEXTURE_HPP

#include "unicorn/utility/SharedMacros.hpp"

#include <cstdint>
#include <string>

namespace unicorn
{
namespace video
{
class Texture
{
public:

    UNICORN_EXPORT Texture();

    UNICORN_EXPORT bool Load(const std::string& path);
    UNICORN_EXPORT bool CopyToGPU();
    UNICORN_EXPORT bool IsLoaded() const;
    UNICORN_EXPORT void Delete();
private:
    int32_t m_width;
    int32_t m_height;
    int32_t m_channels;
    int32_t m_size;
    unsigned char* m_data;
    bool m_initialized;
};
}
}

#endif // UNICORN_VIDEO_TEXTURE_HPP
