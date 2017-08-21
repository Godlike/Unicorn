/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_TEXTURE_HPP
#define UNICORN_VIDEO_TEXTURE_HPP

#include <unicorn/utility/SharedMacros.hpp>

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
    UNICORN_EXPORT Texture(const std::string& path);

    UNICORN_EXPORT bool Load(const std::string& path);
    UNICORN_EXPORT bool IsLoaded() const;
    UNICORN_EXPORT void Delete();
    UNICORN_EXPORT int32_t Size() const;
    UNICORN_EXPORT unsigned char* Data() const;
    UNICORN_EXPORT int32_t Width() const;
    UNICORN_EXPORT int32_t Height() const;
    UNICORN_EXPORT const char* Path() const;
    UNICORN_EXPORT uint32_t GetId() const
    {
        return m_id;
    }
private:
    int32_t m_width;
    int32_t m_height;
    int32_t m_channels;
    int32_t m_size;
    uint32_t m_id;
    unsigned char* m_data;
    const char* m_path;
    bool m_initialized;
};
}
}

#endif // UNICORN_VIDEO_TEXTURE_HPP
