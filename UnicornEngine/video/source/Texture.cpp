/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/utility/Logger.hpp>
#include <unicorn/video/Texture.hpp>
#include <unicorn/utility/asset/SimpleStorage.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace unicorn
{
namespace video
{
Texture::Texture(const std::string& path) : m_width(0)
    , m_height(0)
    , m_channels(0)
    , m_size(0)
    , m_id(0)
    , m_data(nullptr)
    , m_path(path)
    , m_initialized(false)
{
    if(!m_path.empty())
    {
        Load(path);
    }
}

void Texture::Delete()
{
    if(m_initialized && m_data)
    {
        stbi_image_free(m_data);
        m_data = nullptr;
    }
    m_initialized = false;
}

uint32_t Texture::Size() const
{
    if(!m_initialized)
    {
        LOG_WARNING("Texture not loaded!");
    }
    return m_size;
}

uint8_t* Texture::Data() const
{
    if(!m_initialized)
    {
        LOG_WARNING("Texture not loaded!");
    }
    return m_data;
}

uint32_t Texture::Width() const
{
    if(!m_initialized)
    {
        LOG_WARNING("Texture not loaded!");
    }
    return m_width;
}

uint32_t Texture::Height() const
{
    if(!m_initialized)
    {
        LOG_WARNING("Texture not loaded!");
    }
    return m_height;
}

std::string Texture::Path() const
{
    if(!m_initialized)
    {
        LOG_WARNING("Texture not loaded!");
    }
    return m_path;
}

uint32_t Texture::GetId() const
{
    if(!m_initialized)
    {
        LOG_WARNING("Texture not loaded!");
    }
    return m_id;
}

bool Texture::Load(const std::string& path)
{
    Delete();

    m_path = path;

    utility::asset::SimpleStorage& storage = unicorn::utility::asset::SimpleStorage::Instance();
    utility::asset::Handler textureHandler = storage.Get(m_path);

    if(!textureHandler.IsValid())
    {
        LOG_ERROR("Can't find texture - %s", m_path.c_str());
        return false;
    }

    m_data = stbi_load_from_memory(textureHandler.GetContent().GetBuffer().data(),
                                   textureHandler.GetContent().GetBuffer().size(),
                                   reinterpret_cast<int32_t*>(&m_width),
                                   reinterpret_cast<int32_t*>(&m_height),
                                   reinterpret_cast<int32_t*>(&m_channels),
                                   STBI_rgb_alpha);

    m_size = m_width * m_height * 4;

    if(!m_data)
    {
        LOG_ERROR("Failed to load texture image with path - %s", m_path.c_str());
        return false;
    }

    std::hash<std::string> hash_fn;
    m_id = hash_fn(path);
    m_initialized = true;

    return true;
}

bool Texture::IsLoaded() const
{
    return m_initialized;
}
}
}
