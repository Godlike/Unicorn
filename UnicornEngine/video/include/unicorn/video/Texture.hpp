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
     * @brief Loading texture from provided path
     * @param path path to texture
     * @return true if loaded successful and false if not
     */
    UNICORN_EXPORT bool Load(std::string const& path);

    /**
     * @brief Checks if texture was loaded
     * @return true if loaded and false if not
     */
    UNICORN_EXPORT bool IsLoaded() const;

    /**
     * @brief Remove texture data from memory
     */
    UNICORN_EXPORT void Delete();

    /**
     * @brief Returns size of texture
     * @return size of texture, 0 if was not loaded, also shows warning
     */
    UNICORN_EXPORT int32_t Size() const;

    /**
     * @brief Returns pointer to texture data
     * @return pointer to texture data, nullptr if was not loaded, also shows warning
     */
    UNICORN_EXPORT unsigned char* Data() const;

    /**
     * @brief Returns width of texture
     * @return width of texture, 0 if was not loaded, also shows warning
     */
    UNICORN_EXPORT int32_t Width() const;

    /**
     * @brief Returns height of texture
     * @return height of texture, 0 if was not loaded, also shows warning
     */
    UNICORN_EXPORT int32_t Height() const;

    /**
     * @brief Returns path of loaded texture
     * @return path of texture or empty line, if texture was not loaded, also shows warning
     */
    UNICORN_EXPORT std::string Path() const;

    /**
     * @brief Returns Id of loaded texture
     * @return id of texture, 0 if was not loaded, also shows warning
     */
    UNICORN_EXPORT uint32_t GetId() const;
private:
    int32_t m_width;
    int32_t m_height;
    int32_t m_channels;
    int32_t m_size;
    uint32_t m_id;
    unsigned char* m_data;
    std::string m_path;
    bool m_initialized;
};
}
}

#endif // UNICORN_VIDEO_TEXTURE_HPP
