/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_TEXTURE_HPP
#define UNICORN_VIDEO_TEXTURE_HPP

#include <cstdint>
#include <string>

namespace unicorn
{
namespace video
{
/**
 * @brief Holds loaded texture data
 */
class Texture
{
public:
    /**
     * @brief Constructor which loads data from path
     * @param path path to texture
     */
    Texture(std::string const& path = std::string());

    /** @brief Destructs texture, calls FreeData() */
    ~Texture();

    /**
     * @brief Loads texture from provided path
     * @param path path to texture
     * @return true if loaded successful and false if not
     */
    bool Load(std::string const& path);

    /**
     * @brief Checks if texture was loaded
     * @return true if loaded and false if not
     */
    bool IsLoaded() const;

    /**
     * @brief Returns size of texture
     * @return size of texture, 0 if was not loaded
     */
    uint32_t Size() const;

    /**
     * @brief Returns pointer to texture data
     * @return pointer to texture data, nullptr if was not loaded
     */
    uint8_t* Data() const;

    /**
     * @brief Returns width of texture
     * @return width of texture, 0 if was not loaded
     */
    uint32_t Width() const;

    /**
     * @brief Returns height of texture
     * @return height of texture, 0 if was not loaded
     */
    uint32_t Height() const;

    /**
     * @brief Returns path of loaded texture
     * @return const reference to path of texture or empty line, if texture was not loaded
     */
    std::string const& Path() const;

    /**
     * @brief Returns Id of loaded texture
     * @return id of texture, 0 if was not loaded
     */
    uint32_t GetId() const;
private:
    /** @brief Remove texture data from memory */
    void FreeData();

    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_channels;
    uint32_t m_size;
    uint32_t m_id;
    unsigned char* m_data;
    std::string m_path;
    bool m_initialized;
};
}
}

#endif // UNICORN_VIDEO_TEXTURE_HPP
