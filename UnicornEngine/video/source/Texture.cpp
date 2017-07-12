/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/utility/Logger.hpp>
#include <unicorn/video/Texture.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace unicorn
{
namespace video
{
Texture::Texture(): m_width(0)
                  , m_height(0)
                  , m_channels(0)
                  , m_size(0)
                  , m_data(nullptr)
                  , m_initialized(false)
{
}

void Texture::Delete()
{
    if(m_data)
    {
        stbi_image_free(m_data);
        m_data = nullptr;
    }
    m_initialized = false;
}

bool Texture::Load(const std::string& path)
{
    m_data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, STBI_rgb_alpha);
    m_size = m_width * m_height * 4;

    if (!m_data) {
        LOG_ERROR("Failed to load texture image with path - ", path.c_str());
        return false;
    }
    m_initialized = true;
    return true;
}

bool Texture::CopyToGPU()
{

}

bool Texture::IsLoaded() const
{
    return m_initialized;
}
}
}
