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
/**
 * @brief Texture class holds data of loaded texture
 */
class Texture
{
public:
    UNICORN_EXPORT Texture();
    UNICORN_EXPORT Texture(std::string const& path);

    /**
     * @brief 
     * @param path 
     * @return 
     */
    UNICORN_EXPORT bool Load(std::string const& path);

    /**
     * @brief 
     * @return 
     */
    UNICORN_EXPORT bool IsLoaded() const;

    /**
     * @brief 
     */
    UNICORN_EXPORT void Delete();

    /**
     * @brief 
     * @return 
     */
    UNICORN_EXPORT int32_t Size() const;

    /**
     * @brief 
     * @return 
     */
    UNICORN_EXPORT unsigned char* Data() const;

    /**
     * @brief 
     * @return 
     */
    UNICORN_EXPORT int32_t Width() const;

    /**
     * @brief 
     * @return 
     */
    UNICORN_EXPORT int32_t Height() const;

    /**
     * @brief 
     * @return 
     */
    UNICORN_EXPORT const char* Path() const;

    /**
     * @brief 
     * @return 
     */
    UNICORN_EXPORT uint32_t GetId() const;
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
